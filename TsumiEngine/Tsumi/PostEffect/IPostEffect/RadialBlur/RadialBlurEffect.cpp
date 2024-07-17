#include "RadialBlurEffect.h"



// 初期化処理
void RadialBlurEffect::Initialize()
{
	Create();

	// MtlData初期値
	mtlData_.color = Vector4::one;
}


// 描画処理
void RadialBlurEffect::Draw([[maybe_unused]] Camera* camera)
{
	// MtlBufferにMtlを書き込む
	mtlBuffer_.Map();
	mtlBuffer_.WriteData(mtlData_);
	mtlBuffer_.UnMap();

	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void RadialBlurEffect::DrawImGui(std::string name)
{
#ifdef _DEBUG

	// Labelを追加する場合は追加
	label_ = label_ + name;

	if (ImGui::TreeNode((label_ + "RadialBlur").c_str())) {

		ImGui::ColorEdit4("Color", &mtlData_.color.x);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// コマンドコール
void RadialBlurEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::RadialBlur);

	// SRVをコマンドに積む
	SRVManager::SetGraphicsRootDescriptorTable(3, srv_);

	// MtlBufferをコマンドに積む
	mtlBuffer_.CommandCall(4);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
