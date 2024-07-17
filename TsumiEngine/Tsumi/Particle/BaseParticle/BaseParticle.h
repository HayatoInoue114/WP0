#pragma once

#include "../../../Tsumi/Particle/IParticle.h"


/* BaseParticleクラス */
class BaseParticle : public IParticle {

public: // メンバ関数

	// コンストラクタ、デストラクタ
	BaseParticle() {};
	~BaseParticle() {};

	// 初期化処理、更新処理、描画処理
	void Initialize() override;
	void Update() override;
};

