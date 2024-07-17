#include "IEnemyApproachState.h"
#include "../../Enemy/Enemy.h"


// 初期化処理
void IEnemyApproachState::Init(Enemy* enemy)
{
	// Enemyの設定
	enemy_ = enemy;
}


// 更新処理
void IEnemyApproachState::Update()
{
}


// 終了処理
void IEnemyApproachState::Exit()
{
}
