#include "DirectXCommon.h"


/// <summary>
/// DirectXCommonのインスタンス取得
/// </summary>
DirectXCommon* DirectXCommon::GetInstance() {

	static DirectXCommon instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void DirectXCommon::Initialize() {

	/* ----- デバッグレイヤー -----*/
#ifdef _DEBUG

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DirectXCommon::GetInstance()->debugController_)))) {
		// デバッグレイヤーを有効にする
		DirectXCommon::GetInstance()->debugController_->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		DirectXCommon::GetInstance()->debugController_->SetEnableGPUBasedValidation(TRUE);
	}

#endif // _DEBUG


	// DxgiFactoryを生成する
	CreateDxgiFactory();

	// D3D12Deviceを生成する
	CreateDevice();

	// エラーと警告の抑制
	DebugErrorInfoQueue();

	// コマンド関連の生成
	CommandManager::GetInstance()->Initialize();

	// スワップチェーンを生成する
	CreateSwapChain();

	// ディスクリプタヒープの生成
	SetDescriptorHeap();

	// フェンスを作る
	MakeFence();

	// ViewportとScissor
	SetViewport();
	SetScissor();

}


/// <summary>
/// 描画前処理 PostEffect用
/// </summary>
void DirectXCommon::PreDrawForPostEffect() {

	Commands commands = CommandManager::GetInstance()->GetCommands();
	D3D12_RESOURCE_BARRIER barrier{};
	RTVProperty rtv = RTVManager::GetRTV("PostEffect")->GetRTVPrope();
	
	// Barrierを設定する
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = rtv.Resources.Get();
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// TransitionBarrierを張る
	commands.List->ResourceBarrier(1, &barrier);

	// 描画先のRTVとDSVを設定する
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = DirectXCommon::GetInstance()->dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();


	// 描画先のRTVを設定する
	commands.List->OMSetRenderTargets(
		1, &rtv.Handles,
		false,
		&dsvHandle);


	// 指定した色で画面全体をクリアする
	Vector4 color = rtv.color;
	float clearColor[] = { color.x, color.y, color.z, color.w }; // 青っぽい色。RGBAの順


	commands.List->ClearRenderTargetView(
		rtv.Handles,
		clearColor,
		0, nullptr);


	//指定した深度で画面全体をクリアする
	commands.List->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	commands.List->RSSetViewports(1, &DirectXCommon::GetInstance()->viewport_); // Viewportを設定
	commands.List->RSSetScissorRects(1, &DirectXCommon::GetInstance()->scissorRect_); // Scissorを設定

	CommandManager::GetInstance()->SetCommands(commands);
}


/// <summary>
/// 描画後処理 PostEffect用
/// </summary>
void DirectXCommon::PostDrawForPostEffect() {

	Commands commands = CommandManager::GetInstance()->GetCommands();
	D3D12_RESOURCE_BARRIER barrier{};
	RTVProperty rtv = RTVManager::GetRTV("PostEffect")->GetRTVPrope();

	// Barrierを設定する
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = rtv.Resources.Get();
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	// TransitionBarrierを張る
	commands.List->ResourceBarrier(1, &barrier);
}


/// <summary>
/// 描画前処理 SwapChain用
/// </summary>
void DirectXCommon::PreDrawForSwapChain() {

	SwapChains swapChains{};
	swapChains.swapChain = DirectXCommon::GetInstance()->swapChains_.swapChain.Get();
	swapChains.Resources[0] = DirectXCommon::GetInstance()->swapChains_.Resources[0].Get();
	swapChains.Resources[1] = DirectXCommon::GetInstance()->swapChains_.Resources[1].Get();
	Commands commands = CommandManager::GetInstance()->GetCommands();
	D3D12_RESOURCE_BARRIER barrier = DirectXCommon::GetInstance()->barrier_;
	RTVProperty rtv[2] = {
		RTVManager::GetRTV("SwapChain0")->GetRTVPrope(),
		RTVManager::GetRTV("SwapChain1")->GetRTVPrope(),
	};

	// コマンドを積み込んで確定させる
	// これから書き込むバックバッファのインデックスを取得
	UINT backBufferIndex_ = swapChains.swapChain->GetCurrentBackBufferIndex();


	// Barrierを設定する
	// 今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	// Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	// バリアを張る対象のリソース。現在のバックバッファに対して行う
	barrier.Transition.pResource = swapChains.Resources[backBufferIndex_].Get();
	// 遷移前(現在)のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	// 遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	// TransitionBarrierを張る
	commands.List->ResourceBarrier(1, &barrier);

	DirectXCommon::GetInstance()->barrier_ = barrier;


	// 描画先のRTVを設定する
	commands.List->OMSetRenderTargets(
		1, &rtv[backBufferIndex_].Handles,
		false,
		nullptr);

	// 指定した色で画面全体をクリアする
	float clearColor[] = { 
		rtv[backBufferIndex_].color.x, 
		rtv[backBufferIndex_].color.y, 
		rtv[backBufferIndex_].color.z, 
		rtv[backBufferIndex_].color.w }; // 青っぽい色。RGBAの順

	commands.List->ClearRenderTargetView(
		rtv[backBufferIndex_].Handles,
		clearColor,
		0, nullptr);


	commands.List->RSSetViewports(1, &DirectXCommon::GetInstance()->viewport_); // Viewportを設定
	commands.List->RSSetScissorRects(1, &DirectXCommon::GetInstance()->scissorRect_); // Scissorを設定

	CommandManager::GetInstance()->SetCommands(commands);
}


/// <summary>
/// 描画後処理 SwapChain用
/// </summary>
void DirectXCommon::PostDrawForSwapChain() {

	SwapChains swapChains = DirectXCommon::GetInstance()->swapChains_;
	Commands commands = CommandManager::GetInstance()->GetCommands();
	D3D12_RESOURCE_BARRIER barrier = DirectXCommon::GetInstance()->barrier_;

	// 状態を遷移
	// 画面に描く処理はすべて終わり、画面に映すので、状態を遷移
	// 今回はRenderTargetからPresentにする
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	// TransitionBarrierを張る
	commands.List->ResourceBarrier(1, &barrier);

	DirectXCommon::GetInstance()->barrier_ = barrier;


	// コマンドをキックする
	// コマンドリストの内容を確定させる・全てのコマンドを積んでからCloseすること
	HRESULT result;
	result = commands.List->Close();
	assert(SUCCEEDED(result));

	// GPUにコマンドリストの実行を行わせる
	ID3D12CommandList* commandLists[] = { commands.List.Get()};
	commands.Queue->ExecuteCommandLists(1, commandLists);


	// Fenceの値を更新
	DirectXCommon::GetInstance()->fenceValue_++;

	// GPUがここまで辿り着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	commands.Queue->Signal(DirectXCommon::GetInstance()->fence_.Get(), DirectXCommon::GetInstance()->fenceValue_);

	// Fenceの値が指定したSignal値にたどり着いているか確認するか
	// GetCompletedValueの初期値はFence作成時に渡した初期値
	if (DirectXCommon::GetInstance()->fence_->GetCompletedValue() < DirectXCommon::GetInstance()->fenceValue_) {
		// 指定したSignalにたどりついていないので、辿り着くまで待つようにイベントを設定する
		DirectXCommon::GetInstance()->fence_->SetEventOnCompletion(DirectXCommon::GetInstance()->fenceValue_, DirectXCommon::GetInstance()->fenceEvent_);
		// イベントを待つ
		WaitForSingleObject(DirectXCommon::GetInstance()->fenceEvent_, INFINITE);
	}

	swapChains.swapChain->Present(1, 0);

	// 次のフレーム用のコマンドリストを準備
	result = commands.Allocator->Reset();
	assert(SUCCEEDED(result));
	result = commands.List->Reset(commands.Allocator.Get(), nullptr);
	assert(SUCCEEDED(result));

	CommandManager::GetInstance()->SetCommands(commands);
	DirectXCommon::GetInstance()->swapChains_ = swapChains;
}


/// <summary>
/// 解放処理
/// </summary>
void DirectXCommon::Release() {
	CloseHandle(DirectXCommon::GetInstance()->fenceEvent_);
}


/* ---DirectX初期化--- */
/// ここから↓
//////////////////////////////

void DirectXCommon::CreateDxgiFactory() {

	IDXGIFactory7* dxgiFactory = DirectXCommon::GetInstance()->dxgiFactory_.Get();
	IDXGIAdapter4* useAdapter = DirectXCommon::GetInstance()->useAdapter_.Get();

	// DXGIファクトリーの生成
	// HRESULTはWindows系のエラーコードであり、
	// 関数が成功したかどうかをSUCCEEDEDマクロで判定できる
	dxgiFactory = nullptr;
	HRESULT result;
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// 初期化の根本的な部分でエラーが出た場合はプログラムが間違ってるかどうか、
	// どうにもできない場合が多いのでassertにしておく
	assert(SUCCEEDED(result));



	// いい順にアダプタを頼む
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(i,
		DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) !=
		DXGI_ERROR_NOT_FOUND; i++)
	{
		// アダプターの情報を取得する
		DXGI_ADAPTER_DESC3 adapterDesc{};
		result = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(result)); // 取得できないのは一大事

		// ソフトウェアアダプタ出なければ採用！
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			// 採用したアダプタの情報をログに出力。wstringのほうなので注意
			Log(ConvertString(std::format(L"USE Adapater:{}\n", adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr; // ソフトウェアアダプタの場合は見なかったことにする
	}
	// 適切なアダプタが見つからなかったので起動できない
	assert(useAdapter != nullptr);

	DirectXCommon::GetInstance()->dxgiFactory_ = dxgiFactory;
	DirectXCommon::GetInstance()->useAdapter_ = useAdapter;
}



/* ----- D3D12Deviceを生成する ----- */

void DirectXCommon::CreateDevice() {

	// 昨日レベルとログ出力用の文字列
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};

	const char* featureLevelStrings[] = { "12.2", "12.1", "12.0" };


	// 高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		// 採用したアダプターでデバイスを生成
		HRESULT result;
		result = D3D12CreateDevice(DirectXCommon::GetInstance()->useAdapter_.Get(), featureLevels[i], IID_PPV_ARGS(&DirectXCommon::GetInstance()->device_));

		// 指定した昨日レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(result)) {
			// 生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
			break;
		}
	}

	// デバイスの生成がうまく行かなかったので起動できない
	assert(DirectXCommon::GetInstance()->device_ != nullptr);
	Log("Complete create D3D12Device!!!\n"); // 初期化完了のログを出す

}

//////////////////////////////
/// ここまで↑
/* ---DirectX初期化--- */



/* ----- エラーと警告の抑制 ----- */

void DirectXCommon::DebugErrorInfoQueue() {

#ifdef _DEBUG

	ID3D12InfoQueue* infoQueue = DirectXCommon::GetInstance()->infoQueue_.Get();
	D3D12_INFO_QUEUE_FILTER filter = DirectXCommon::GetInstance()->filter_;

	if (SUCCEEDED(DirectXCommon::GetInstance()->device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {

		// ヤバいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);

		// エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		// 警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);


		// 抑制するメッセージのID
		D3D12_MESSAGE_ID denyIds_[] = {
			// Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			// https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities_[] = { D3D12_MESSAGE_SEVERITY_INFO };
		filter.DenyList.NumIDs = _countof(denyIds_);
		filter.DenyList.pIDList = denyIds_;
		filter.DenyList.NumSeverities = _countof(severities_);
		filter.DenyList.pSeverityList = severities_;
		// 指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		DirectXCommon::GetInstance()->infoQueue_ = infoQueue;
		DirectXCommon::GetInstance()->filter_ = filter;


		// 解放
		DirectXCommon::GetInstance()->infoQueue_->Release();
	}

#endif // _DEBUG
}



/* ----- コマンドキューを生成する ----- */

//void DirectXCommon::CreateCommandQueue() {
//
//	// コマンドキューを生成する
//	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
//	HRESULT result;
//	result = DirectXCommon::GetInstance()->device_->CreateCommandQueue(
//		&commandQueueDesc,
//		IID_PPV_ARGS(&DirectXCommon::GetInstance()->commands_.Queue));
//
//
//	// コマンドキューの生成がうまくいかなかったので起動できない
//	assert(SUCCEEDED(result));
//}



/* ----- コマンドアロケータを生成する ----- */
//void DirectXCommon::CreateCommandAllocator() {
//
//	// コマンドアロケータを生成する
//	HRESULT result;
//	result = DirectXCommon::GetInstance()->device_->CreateCommandAllocator(
//		D3D12_COMMAND_LIST_TYPE_DIRECT,
//		IID_PPV_ARGS(&DirectXCommon::GetInstance()->commands_.Allocator));
//
//
//	// コマンドアロケータの生成がうまくいかなかったので起動できない
//	assert(SUCCEEDED(result));
//}



/* ----- コマンドリストを生成する ----- */

//void DirectXCommon::CreateCommandList() {
//
//	// コマンドリストを生成する
//	HRESULT result;
//	result = DirectXCommon::GetInstance()->device_->CreateCommandList(
//		0, D3D12_COMMAND_LIST_TYPE_DIRECT,
//		DirectXCommon::GetInstance()->commands_.Allocator.Get(),
//		nullptr,
//		IID_PPV_ARGS(&DirectXCommon::GetInstance()->commands_.List));
//
//
//	// コマンドリストの生成がうまくいかなかったので起動できない
//	assert(SUCCEEDED(result));
//}



/* ----- スワップチェーンを生成する ----- */

void DirectXCommon::CreateSwapChain() {

	HWND hwnd_ = WinApp::GetInstance()->GetHwnd();
	SwapChains swapChains = DirectXCommon::GetInstance()->swapChains_;

	swapChains.Desc.Width = WinApp::kWindowWidth;	  // 画面の幅、ウィンドウのクライアント領域を同じものにしてく
	swapChains.Desc.Height = WinApp::kWindowHeight; // 画面の高さ、ウィンドウのクライアント領域を同じものにしておく
	swapChains.Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色の形式
	swapChains.Desc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChains.Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画のターゲットとして利用する
	swapChains.Desc.BufferCount = 2; // ダブルバッファ
	swapChains.Desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // モニタにうつしたら、中身を破棄

	// コマンドキュー、ウィンドウハンドル、設定を渡して生成する
	HRESULT result;
	result = DirectXCommon::GetInstance()->dxgiFactory_->CreateSwapChainForHwnd(
		CommandManager::GetInstance()->GetQueue(),
		hwnd_,
		&swapChains.Desc,
		nullptr,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChains.swapChain.GetAddressOf()));

	assert(SUCCEEDED(result));

	DirectXCommon::GetInstance()->swapChains_ = swapChains;
}



/* ----- ディスクリプタヒープの生成 ----- */

void DirectXCommon::SetDescriptorHeap() {

	RTVManager::GetInstance()->SetDescriptorHeap(
		CreateDescriptorHeap(DirectXCommon::GetInstance()->device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 5, false)
	);

	DirectXCommon::GetInstance()->srvDescriptorHeap_ = CreateDescriptorHeap(
		DirectXCommon::GetInstance()->device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);


	// SwapChainからResourceを引っ張ってくる
	CreateSwapChainResources();


	// RTVを作る
	CreateRTV();


	// depthStencilResourceを作る
	CreateDepthStencilResource();
}



/* ----- SwapChainからResourceを引っ張ってくる ----- */

void DirectXCommon::CreateSwapChainResources() {

	HRESULT result;
	result = DirectXCommon::GetInstance()->swapChains_.swapChain->GetBuffer(0, IID_PPV_ARGS(&DirectXCommon::GetInstance()->swapChains_.Resources[0]));

	// うまく取得できなければ起動できない
	assert(SUCCEEDED(result));


	result = DirectXCommon::GetInstance()->swapChains_.swapChain->GetBuffer(1, IID_PPV_ARGS(&DirectXCommon::GetInstance()->swapChains_.Resources[1]));

	// うまく取得できなければ起動できない
	assert(SUCCEEDED(result));
}



/* ----- RTVを作る ----- */

void DirectXCommon::CreateRTV()
{
	ComPtr<ID3D12Device> device = DirectXCommon::GetInstance()->device_;
	SwapChains swapChains = DirectXCommon::GetInstance()->GetSwapChains();

	// Heapの設定
	RTVManager::GetInstance()->SetDescriptorHeap(CreateDescriptorHeap(device.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RTV_Index_Max, false));

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 出力結果をSRGBに変換して書き込む
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2sテクスチャとして書き込む
	RTVManager::GetInstance()->SetDesc(desc); // デスクの設定

	// クリアカラー
	Vector4 clearColor = { 0.1f, 0.25f, 0.5f, 1.0f }; // 青っぽい色。RGBAの順

	// ディスクリプタの先頭を取得する
	D3D12_CPU_DESCRIPTOR_HANDLE startHandle;
	startHandle = RTVManager::GetInstance()->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();


	RTVProperty rtvPrope[4]{};
	// 1つ目
	rtvPrope[0].Handles = startHandle;
	device->CreateRenderTargetView(swapChains.Resources[0].Get(), &desc, rtvPrope[0].Handles);
	rtvPrope[0].Resources = swapChains.Resources[0].Get();
	rtvPrope[0].color = clearColor;
	
	// 2つ目
	rtvPrope[1].Handles.ptr = rtvPrope[0].Handles.ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(swapChains.Resources[1].Get(), &desc, rtvPrope[1].Handles);
	rtvPrope[1].Resources = swapChains.Resources[1].Get();
	rtvPrope[1].color = clearColor;
	
	// 3つ目
	rtvPrope[2].Handles.ptr = rtvPrope[1].Handles.ptr + device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	auto renderTextureResource = CreateResource::CreateRenderTextureResource(
		WinApp::kWindowWidth,
		WinApp::kWindowHeight,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		clearColor);
	device->CreateRenderTargetView(renderTextureResource.Get(), &desc, rtvPrope[2].Handles);
	rtvPrope[2].Resources = renderTextureResource.Get();
	rtvPrope[2].color = clearColor;


	// DescriptorHandleとDescriptorHeap
	typedef struct D3D12_CPU_DESCRIPTOR_HANDLE {
		SIZE_T ptr;
	}D3D12_CPU_DESCRIPTOR_HANDLE;

	
	// RTVManagerに設定する
	RTVManager::AddRTV("SwapChain0", rtvPrope[0]);
	RTVManager::AddRTV("SwapChain1", rtvPrope[1]);
	RTVManager::AddRTV("PostEffect", rtvPrope[2]);
}



// 状態を遷移する
void DirectXCommon::ChanegResourceState() {

	// 画面に描く処理はすべて終わり、画面に映すので、状態を遷移
	// 今回はRenderTargetからPresentにする
	DirectXCommon::GetInstance()->barrier_.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	DirectXCommon::GetInstance()->barrier_.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	// TransitionBarrierを張る
	CommandManager::GetInstance()->GetList()->ResourceBarrier(1, &DirectXCommon::GetInstance()->barrier_);
}



/* ----- Fenceを生成する ----- */

void DirectXCommon::MakeFence() {

	// 初期値0でFenceを作る
	HRESULT result;
	result = DirectXCommon::GetInstance()->device_->CreateFence(DirectXCommon::GetInstance()->fenceValue_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&DirectXCommon::GetInstance()->fence_));
	assert(SUCCEEDED(result));
	// FenceのSignalを待つためのイベントを作成する
	HANDLE fenceEvent;
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}



/* ----- ViewportとScissor ----- */

void DirectXCommon::SetViewport() {

	// ビューポート
	// クライアント領域のサイズと一緒にして画面全体に表示
	DirectXCommon::GetInstance()->viewport_.Width = float(WinApp::kWindowWidth);
	DirectXCommon::GetInstance()->viewport_.Height = float(WinApp::kWindowHeight);
	DirectXCommon::GetInstance()->viewport_.TopLeftX = 0;
	DirectXCommon::GetInstance()->viewport_.TopLeftY = 0;
	DirectXCommon::GetInstance()->viewport_.MinDepth = 0.0f;
	DirectXCommon::GetInstance()->viewport_.MaxDepth = 1.0f;
}

void DirectXCommon::SetScissor() {

	// シザー矩形
	// 基本的にビューポートと同じ矩形が構成されるようにする
	DirectXCommon::GetInstance()->scissorRect_.left = 0;
	DirectXCommon::GetInstance()->scissorRect_.right = WinApp::kWindowWidth;
	DirectXCommon::GetInstance()->scissorRect_.top = 0;
	DirectXCommon::GetInstance()->scissorRect_.bottom = WinApp::kWindowHeight;
}



ID3D12DescriptorHeap* DirectXCommon::CreateDescriptorHeap(
	ID3D12Device* device,
	D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors,
	bool shaderVisible) {

	DirectXCommon::GetInstance()->DescriptorHeapDesc_.Type = heapType;
	DirectXCommon::GetInstance()->DescriptorHeapDesc_.NumDescriptors = numDescriptors;
	DirectXCommon::GetInstance()->DescriptorHeapDesc_.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT result;
	result = device->CreateDescriptorHeap(&DirectXCommon::GetInstance()->DescriptorHeapDesc_, IID_PPV_ARGS(&DirectXCommon::GetInstance()->descriptorHeap_));
	//ディスクリプタヒープの生成ができないので起動できない
	assert(SUCCEEDED(result));
	return DirectXCommon::GetInstance()->descriptorHeap_.Get();
}



// Textureの深度の設定をしていく
ComPtr<ID3D12Resource> DirectXCommon::CreateDepthStencilTexturerResource(int32_t width, int32_t height) {

	// 生成するResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = width; // Textureの幅
	resourceDesc.Height = height; // Textureの高さ
	resourceDesc.MipLevels = 1; // mipmapの数
	resourceDesc.DepthOrArraySize = 1; // 奥行き or 配列Textureの配列数
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	resourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知


	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る


	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる


	// Resourceの生成
	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT result;
	result = DirectXCommon::GetInstance()->device_->CreateCommittedResource(
		&heapProperties, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Hepaの特殊な設定。特になし
		&resourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&resource)); // 作成するResourceポインタへのポインタ

	assert(SUCCEEDED(result));

	return resource;
}



// depthStencilResourceを作る
void DirectXCommon::CreateDepthStencilResource() {

	// DepthStencilTextureをウィンドウサイズで作成
	DirectXCommon::GetInstance()->depthStencilResource_ = CreateDepthStencilTexturerResource(
		WinApp::kWindowWidth, 
		WinApp::kWindowHeight);


	// DSV用のヒープでディスクリプタのの数は1。DSVはShader内で触るものではないので、ShaderVisibleはfalse
	DirectXCommon::GetInstance()->dsvDescriptorHeap_ =
		CreateDescriptorHeap(DirectXCommon::GetInstance()->device_.Get(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);


	// DSVの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的はResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture


	// DSVHeapの先頭にDSVを作る
	DirectXCommon::GetInstance()->device_->CreateDepthStencilView(
		DirectXCommon::GetInstance()->depthStencilResource_.Get(), 
		&dsvDesc, 
		DirectXCommon::GetInstance()->dsvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart());
}


// DescriptorHandleを取得する
D3D12_CPU_DESCRIPTOR_HANDLE DirectXCommon::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DirectXCommon::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (static_cast<unsigned long long>(descriptorSize) * index);
	return handleGPU;
}