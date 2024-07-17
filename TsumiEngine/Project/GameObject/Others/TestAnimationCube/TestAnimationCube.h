#pragma once

#include "../../GameObject.h"

/* TestAnimationCubeクラス */
class TestAnimationCube {

public:

	// コンストラクタデストラクタ
	TestAnimationCube() {};
	~TestAnimationCube() {};

	// 初期化処理、更新処理、描画処理
	void Init();
	void Update();
	void Draw(Camera* camera);

private:

	// テストモデル
	unique_ptr<Model> testModel_;
	WorldTransform wt_{};
	Animation testAnimation_{};
	float animationtime = 0.0f;
};

