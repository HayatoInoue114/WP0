#pragma once

#include "../../GameObject.h"

/* TestHumanクラス */
class TestHuman {

public: 

	// コンストラクタ、デストラクタ	
	TestHuman() {};
	~TestHuman() {};

	// 初期化処理、更新処理、描画処理
	void Init();
	void Update();
	void Draw(Camera* camera);

private:

	//  ウォークモデル
	unique_ptr<Model> walkModel_;
	WorldTransform walkWt_{};
	float animTimeA_ = 0.0f;
	Skeleton walkSkeleton_{};
	SkinCluster walkSkinCluster_{};

	// スニークモデル
	unique_ptr<Model> sneakModel_;
	WorldTransform sneakWt_{};
	float animTimeB_ = 0.0f;
	Skeleton sneakSkeleton_{};
	SkinCluster sneakSkinCluster_{};

	//  シンプルモデル
	unique_ptr<Model> simpleModel_;
	WorldTransform simpleWt_{};
	float animTimeC_ = 0.0f;
	Skeleton simpleSkeleton_{};
	SkinCluster simpleSkinCluster_{};

	// アニメーションタイプ
	Animation walkAnim_{};
	Animation sneakAnim_{};
	Animation simpleAnim_{};
};

