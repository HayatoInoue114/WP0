#include "IEnemySpawnState.h"
#include "../../Enemy/Enemy.h"


// 初期化処理
void IEnemySpawnState::Init(Enemy* enemy)
{
	// Enemyの設定
	enemy_ = enemy;

	// スケールのイージングにかかる時間の設定。1秒
	scaleEaseTime_.Start(0.0f, 60.0f);
}


// 更新処理
void IEnemySpawnState::Update()
{
	// SRTの取得
	srt_ = enemy_->GetSRT();

	// スケールをイージングにかける
	if (OutElasticScale()) {

		// trueでチェンジステート
		enemy_->ChangeState(EnemyState::APPROACH);
	}

	// SRTの再設定
	enemy_->SetSRT(srt_);
}


// 終了処理
void IEnemySpawnState::Exit()
{
}


// スケールをイージングにかける
bool IEnemySpawnState::OutElasticScale()
{
	// タイマーの更新
	scaleEaseTime_.Update();

	// 設定するスケール
	float setScale = 
		scalePair_.first + (scalePair_.second - scalePair_.first) * Ease::OutElastic(scaleEaseTime_.GetRatio());

	srt_.scale = { setScale, setScale, setScale };

	// イージングが終了したら、クリアしてtrueを返す
	if (scaleEaseTime_.IsFinish()) {

		scaleEaseTime_.Clear();
		return true;
	}

	return false;
}
