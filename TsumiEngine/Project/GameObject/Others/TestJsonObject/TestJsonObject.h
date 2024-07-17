#pragma once

#include "../../GameObject.h"
#include "../../../../Tsumi/FileManager/FileManager.h"

/* TestJsonObjectクラス */
class TestJsonObject {

public:

	// コンストラクタ、デストラクタ
	TestJsonObject() {};
	~TestJsonObject() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void Draw(Camera* camera);

private:


private:

	// テストモデル : 壁
	unique_ptr<Model> wallModel_;
	WorldTransform wallWt_{};

	// テストモデル : 棒
	unique_ptr<Model> bouModel_;
	WorldTransform bouWt_{};

	// テストモデル : 具
	unique_ptr<Model> guModel_;
	WorldTransform guWt_{};
};

