#include "VignettingEffect.h"



// 初期化処理
void VignettingEffect::Initialize()
{
	Create();
}


// 描画処理
void VignettingEffect::Draw([[maybe_unused]] Camera* camera)
{
	// MtlBufferにMtlを書き込む
	mtlBuffer_.Map();
	mtlBuffer_.WriteData(mtlData_);
	mtlBuffer_.UnMap();

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void VignettingEffect::DrawImGui(std::string name)
{
#ifdef _DEBUG

	// Labelを追加する場合は追加
	label_ = label_ + name;

	if (ImGui::TreeNode((label_ + "Vignetting").c_str())) {

		ImGui::ColorEdit4("Color", &mtlData_.color.x);
		ImGui::DragFloat("Intensity", &mtlData_.intensity, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Center", &mtlData_.center, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat2("Radisu", &mtlData_.radius.x, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Smoothness", &mtlData_.smoothness, 0.01f, 0.01f, 1.0f);
		ImGui::DragFloat("Roundness", &mtlData_.roundness, 0.01f, 0.0f, 1.0f);
		ImGui::TreePop();
	}

#endif // _DEBUG
}


// コマンドコール
void VignettingEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Vignetting);

	// SRVをコマンドに積む
	SRVManager::SetGraphicsRootDescriptorTable(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.CommandCall(4);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
