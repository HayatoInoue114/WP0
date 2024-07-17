#include "Tsumi.h"

/// <summary>
/// インスタンスの取得
/// </summary>
Tsumi* Tsumi::GetInstance() {
	static Tsumi instance;
	return &instance;
}


/// <summary>
/// 初期化処理
/// </summary>
void Tsumi::Initialize() {

	// WinAppの初期化処理
	WinApp::Initialize(L"LE3B_03_イノウエ_ハヤト");

	// DirectXCommonの初期化処理
	DirectXCommon::Initialize();

	// ShaderManagerの初期化処理
	ShaderManager::Initialize();

	// PipeLineManagerの初期化処理
	PipeLineManager::Initialize();

	// TextureManagerの初期化処理
	TextureManager::Initialize();

	// ImGuiの初期化処理
	ImGuiManager::Initialize();

	// Inputの初期化処理
	Input::GetInstance()->Initialize();

	// Audioの初期化処理
	Audio::Initialize();

	// DescriptorManagerの初期化処理
	//DescriptorManager::Initialize();
	SRVManager::Initialize();

	// DrawSystemの初期化処理
	DrawSystem::GetInstance()->Initialize();

	// FileManagerの初期化処理
	FileManager::GetInstance()->Initialize();
}


/// <summary>
/// 解放処理
/// </summary>
void Tsumi::Finalize() {

	Audio::Finalize();
	TextureManager::Finalize();
	//DescriptorManager::Clear();
	SRVManager::Clear();
}


/// <summary>
/// フレームの開始
/// </summary>
void Tsumi::BeginFlame() {

	ImGuiManager::BeginFrame();
	Input::GetInstance()->BeginFrame();
	//DescriptorManager::BeginFrame();
	RTVManager::BeginFrame();
	SRVManager::BeginFrame();
	PipeLineManager::PipeLineTypeReset();
}


/// <summary>
/// フレームの終了
/// </summary>
void Tsumi::EndFlame() {

	// ImGuiの描画処理
	ImGuiManager::EndFrame();
}


/// <summary>
/// メッセージの処理
/// </summary>
bool Tsumi::ProcessMessage() {

	if (WinApp::ProcessMessage()) {
		return true;
	}
	else {

		return false;
	}
}