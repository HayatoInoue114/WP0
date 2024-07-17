#include "DissolveEffect.h"



// 初期化処理
void DissolveEffect::Initialize()
{
	Create();

	mtlData_.maskTexture = TextureManager::LoadTexture("Texture", "noise0.png");
}


// 描画処理
void DissolveEffect::Draw([[maybe_unused]] Camera* camera)
{
	// MtlBufferにMtlを書き込む
	mtlBuffer_.Map();
	mtlBuffer_.WriteData(mtlData_);
	mtlBuffer_.UnMap();

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void DissolveEffect::DrawImGui(std::string name)
{
#ifdef _DEBUG

	// Labelを追加する場合は追加
	label_ = label_ + name;

	if (ImGui::TreeNode((label_ + "Dissolve").c_str())) {

		ImGui::ColorEdit4("Color", &mtlData_.color.x);
		ImGui::DragFloat("Threshold", &mtlData_.threshold, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Thinkness", &mtlData_.thinkness, 0.01f, 0.0f, 1.0f);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// コマンドコール
void DissolveEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Dissolve);

	// SRVをコマンドに積む
	SRVManager::SetGraphicsRootDescriptorTable(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.CommandCall(4);

	// MaksTexture
	SRVManager::SetGraphicsRootDescriptorTable(5, mtlData_.maskTexture);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
