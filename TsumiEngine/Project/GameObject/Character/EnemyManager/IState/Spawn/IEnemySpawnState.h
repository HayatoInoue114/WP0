#pragma once

#include "../../../../GameObject.h"
#include "../IEnemyState.h"

#include <memory>
#include <utility>


/* SpawnState派生クラス */
class IEnemySpawnState : public IEnemyState {

public:

	// コンストラクタ
	IEnemySpawnState() {};

	// 初期化、更新、終了
	void Init(Enemy* enemy) override;
	void Update() override;
	void Exit() override;


private:

	// スケールをイージングにかける
	bool OutElasticScale();

private:

	// SRT
	SRT srt_{};

	// スケールの開始値と終了値。(0.0f, 1.0f)
	std::pair<float, float> scalePair_ = { 0.0f, 1.0f };

	// スケールのイージングにかかるタイマー
	Timer scaleEaseTime_{};
};

