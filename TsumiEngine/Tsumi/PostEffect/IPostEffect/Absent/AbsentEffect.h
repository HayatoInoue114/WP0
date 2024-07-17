#pragma once

#include "../IPostEffect.h"

/* AbsentEffectクラス */
class AbsentEffect : public IPostEffect<AbsentMtl> {

public:

	// コンストラクタ、デストラクタ
	AbsentEffect() {};
	~AbsentEffect() {};

	// 初期化処理、描画処理
	void Initialize() override;
	void Draw([[maybe_unused]] Camera* camera) override;

	// Imguiの描画
	void DrawImGui(std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};
