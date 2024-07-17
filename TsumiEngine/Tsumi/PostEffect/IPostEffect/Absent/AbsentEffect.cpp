#include "AbsentEffect.h"



// 初期化処理
void AbsentEffect::Initialize()
{
	Create();

}


// 描画処理
void AbsentEffect::Draw([[maybe_unused]] Camera* camera)
{
	// コマンドコール
	CommandCall();
}


// ImGuiの描画
void AbsentEffect::DrawImGui(std::string name)
{
#ifdef _DEBUG

	// Labelを追加する場合は追加
	label_ = label_ + name;
	if (ImGui::TreeNode((label_ + "Absent").c_str())) {
		ImGui::TreePop();
	}

#endif // _DEBUG
}


// コマンドコール
void AbsentEffect::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineの設定
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Absent);

	// SRVをコマンドに積む
	SRVManager::SetGraphicsRootDescriptorTable(3, srv_);

	// 描画
	commands.List->DrawInstanced(3, 1, 0, 0);
}
