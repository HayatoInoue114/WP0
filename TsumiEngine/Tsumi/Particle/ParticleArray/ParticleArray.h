#pragma once

#include "../../../Project/Math/MyMath.h"
#include "../IParticle.h"

/* ParticleArrayクラス */
class ParticleArray {

public:

	// コンストラクタ、デストラクタ
	ParticleArray() {};
	~ParticleArray() {};

	// 更新処理
	void Update();

	// 粒子配列に粒子追加
	IParticle* const push_back(unique_ptr<IParticle> addParticle);

	// 粒子配列の取得
	const auto& GetParticleArray() { return this->particles_; }


private:

	// 粒子配列
	list<unique_ptr<IParticle>> particles_;
};

