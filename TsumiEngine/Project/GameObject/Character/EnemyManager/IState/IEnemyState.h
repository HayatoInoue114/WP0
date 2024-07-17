#pragma once


// ステートの種類
enum EnemyState {
	SPAWN,
	APPROACH,
	DEATH,
	Count,
};

// Enemyの前方宣言
class Enemy;

/* IEnemyState基底クラス */
class IEnemyState {

public:

	// 仮想デストラクタ
	virtual ~IEnemyState() = default;

	// 初期化、更新、終了
	virtual void Init(Enemy* enemy) = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;


protected:

	Enemy* enemy_ = nullptr;
};


