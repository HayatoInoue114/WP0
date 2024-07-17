#include "IEnemyDeathState.h"
#include "../../Enemy/Enemy.h"


// 初期化処理
void IEnemyDeathState::Init(Enemy* enemy)
{
	// Enemyの設定
	enemy_ = enemy;

	// イージングのタイマー時間設定。1.5秒
	posEaseTime_.Start(0.0f, 90.0f);

	// イージングの開始座標と終了座標
	posPair_ = {
		enemy_->GetPosition().y,
		enemy_->GetPosition().y - 20,
	};
}


// 更新処理
void IEnemyDeathState::Update()
{
	// SRTの取得
	srt_ = enemy_->GetSRT();

	// スケールをイージングにかける
	if (InOutBackPos()) {

		// trueでデスフラグを立てる
		enemy_->SetDeadFlag(true);
	}

	// SRTの再設定
	enemy_->SetSRT(srt_);
}


// 終了処理
void IEnemyDeathState::Exit()
{
}


// ポジションをイージングにかける
bool IEnemyDeathState::InOutBackPos()
{
	// タイマーの更新
	posEaseTime_.Update();

	srt_.translate.y = 
		posPair_.first + (posPair_.second - posPair_.first) * Ease::InOutBack(posEaseTime_.GetRatio());


	// イージングが終了したら、クリアしてtrueを返す
	if (posEaseTime_.IsFinish()) {

		posEaseTime_.Clear();
		return true;
	}

	return false;
}
