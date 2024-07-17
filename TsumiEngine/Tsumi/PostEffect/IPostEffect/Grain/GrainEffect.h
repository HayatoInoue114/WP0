#pragma once

#include "../IPostEffect.h"

/* GrainEffectクラス */
class GrainEffect : public IPostEffect<GrainMtl> {

public:

	// コンストラクタ、デストラクタ
	GrainEffect() {};
	~GrainEffect() {};

	// 初期化処理、描画処理
	void Initialize() override;
	void Draw([[maybe_unused]] Camera* camera) override;

	// Imguiの描画
	void DrawImGui(std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};

