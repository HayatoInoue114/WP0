#pragma once

#include "../../Tsumi/CommandManager/CommandManager.h"
#include "../../Tsumi/CreateResource/CreateResource.h"
#include "../../Tsumi/PipeLineManager/PipeLineManager.h"

#include "../../GameObject.h"


/* Skyboxクラス */
class Skybox {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	Skybox() = default;
	~Skybox() = default;
	Skybox(const Skybox&) = delete;
	const Skybox& operator=(const Skybox&) = delete;


public: // メンバ関数

	// インスタンスの取得
	static Skybox* GetInstance() {
		static Skybox instance;
		return &instance;
	}

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void Draw(Camera* camera);


private:

	// コマンドコール
	void CommandCall(Camera* camera);


private: // メンバ変数

	// Transform
	WorldTransform transform_{};
	float initScale_ = 1.0f;

	// 使用するTextureHandle
	uint32_t texture_ = 0;

	// カラー
	Vector4 color_{};

	// 定数の頂点数とインデックス数
	const size_t kNumVertices_ = 24; // 各面4頂点、6面で計24頂点
	const size_t kNumIndices_ = 36;  // 各面6インデックス、6面で計36インデックス

	// リソース
	ResourcePeroperty resource_{};
};

