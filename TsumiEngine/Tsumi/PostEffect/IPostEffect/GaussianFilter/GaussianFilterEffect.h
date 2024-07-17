#pragma once

#include "../IPostEffect.h"

/* GaussianFilterEffectクラス */
class GaussianFilterEffect : public IPostEffect<GaussianFilterMtl> {

public:

	// コンストラクタ、デストラクタ
	GaussianFilterEffect() {};
	~GaussianFilterEffect() {};

	// 初期化処理、描画処理
	void Initialize() override;
	void Draw([[maybe_unused]] Camera* camera) override;

	// Imguiの描画
	void DrawImGui(std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};

