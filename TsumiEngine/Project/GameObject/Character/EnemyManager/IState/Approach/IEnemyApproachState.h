#pragma once

#include "../../../../GameObject.h"
#include "../IEnemyState.h"


/* ApproachState派生クラス */
class IEnemyApproachState : public IEnemyState {

public:

	// コンストラクタ、
	IEnemyApproachState() {};

	// 初期化、更新、終了
	void Init(Enemy* enemy) override;
	void Update() override;
	void Exit() override;

private:

};