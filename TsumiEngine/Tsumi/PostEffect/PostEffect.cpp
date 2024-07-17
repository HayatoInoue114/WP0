#include "PostEffect.h"



// 初期化処理
void PostEffect::Initialize()
{
	// リソースの作成
	resource_.Material = CreateResource::CreateBufferResource(sizeof(Material));

	ComPtr<ID3D12Resource> stv = RTVManager::GetRTV("PostEffect")->GetRTVPrope().Resources.Get();
	//srv_ = DescriptorManager::CreateRenderTextureSRV(stv);
	srv_ = SRVManager::CreatePostEffectSRV(stv);
}


// 描画処理
void PostEffect::Draw()
{
	Material* materialData = nullptr;

	// 書き込みができるようにする
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	// 色
	materialData->color = Vector4::one;

	// コマンドコール
	CommandCall();
}


// コマンドコール
void PostEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// Materialの設定
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// DescriptorTableの設定
	//DescriptorManager::SetGraphicsRootDescriptorTable(3, srv_);
	SRVManager::SetGraphicsRootDescriptorTable(3, srv_);

	// 頂点3つ描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
