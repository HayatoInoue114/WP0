#pragma once

#include "../IPostEffect.h"

/* ColorGradingEffectクラス */
class ColorGradingEffect : public IPostEffect<ColorGradingMtl> {

public:

	// コンストラクタ、デストラクタ
	ColorGradingEffect() {};
	~ColorGradingEffect() {};

	// 初期化処理、描画処理
	void Initialize() override;
	void Draw([[maybe_unused]] Camera* camera) override;

	// Imguiの描画
	void DrawImGui(std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};
