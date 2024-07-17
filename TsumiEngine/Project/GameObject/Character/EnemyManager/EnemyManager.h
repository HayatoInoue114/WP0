#pragma once

#include <list>
#include <memory>

#include "../../GameObject.h"

#include "Enemy/Enemy.h"


// Player前方宣言
class Player;


/* EnemyManagerクラス */
class EnemyManager {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	EnemyManager() {};
	~EnemyManager() {};

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void Draw3D(Camera* camera);

	// 新しいEnemyを追加する
	void AddNewEnemy();

#pragma region Accessor アクセッサ

	// Playerの設定
	void SetPlayer(Player* setPlayer) { this->player_ = setPlayer; }

	// EnemyListの取得
	std::list<std::shared_ptr<Enemy>>& GetEnemyList() { return this->enemyList_; }

#pragma endregion 


private:

	// 新しいEnemyを生成する
	void CreateNewEnemy();

	// エネミーカウントチェック
	void EnemyCountCheck();


private: // メンバ変数

	// Player
	Player* player_ = nullptr;

	// FlagModel
	std::unique_ptr<Model> flagModel_;

	// Transform
	WorldTransform transform_{};

	// EnemyのLIst配列
	std::list<std::shared_ptr<Enemy>> enemyList_;

	// Vector3Scope。エネミーの湧き範囲
	ScopeVec3 scope3_{};

	// エネミーの最低数
	int enemyMinInstance_ = 0;

	// エネミーのカウントチェックタイマー
	Timer enemyCountCheckTime_;
};

