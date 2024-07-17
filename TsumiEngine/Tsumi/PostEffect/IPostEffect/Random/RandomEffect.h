#pragma once

#include "../IPostEffect.h"

/* RandomEffectクラス */
class RandomEffect : public IPostEffect<RandomMtl> {

public:

	// コンストラクタ、デストラクタ
	RandomEffect() {};
	~RandomEffect() {};

	// 初期化処理、描画処理
	void Initialize() override;
	void Draw([[maybe_unused]] Camera* camera) override;

	// Imguiの描画
	void DrawImGui(std::string name = "") override;

protected:

	// コマンドコール
	void CommandCall() override;
};