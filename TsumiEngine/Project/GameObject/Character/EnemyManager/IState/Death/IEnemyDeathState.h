#pragma once

#include "../../../../GameObject.h"
#include "../IEnemyState.h"


/* DeathState派生クラス */
class IEnemyDeathState : public IEnemyState {

public:

	// コンストラクタ
	IEnemyDeathState() {};

	// 初期化、更新、終了
	void Init(Enemy* enemy) override;
	void Update() override;
	void Exit() override;


private:

	// ポジションをイージングにかける
	bool InOutBackPos();

private:

	// SRT
	SRT srt_{};

	// 開始値と終了値
	std::pair<float, float> posPair_;

	// スケールのイージングにかかるタイマー
	Timer posEaseTime_{};

};