#pragma once

#include "../../GameObject.h"

/* TestObjectクラス */
class TestObject {

public:

	// コンストラクタ、デストラクタ
	TestObject() {};
	~TestObject() {};

	// 初期化処理、更新処理、描画処理
	void Init();
	void Update();
	void Draw(Camera* camera);

private:

	// テストモデル
	unique_ptr<Model> testModel_;
	WorldTransform wt_{};
};

