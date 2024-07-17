#include "BaseParticle.h"



// 初期化処理
void BaseParticle::Initialize()
{
	wt_.Initialize();
	uv_.Initialize();

	wt_.srt.translate = Vector3::zero;

	perSecond_ = 1;

	direction_ = Vector3::zero;

	velocity_ = Vector3::one / 5.0f;

	minLifetime_ = 3.0f;
	maxLifetime_ = 6.0f;

	color_ = Vector4::one;

	isActive_ = true;
}


// 更新処理
void BaseParticle::Update()
{
	wt_.UpdateMatrix();
	uv_.UpdateMatrix();

	//wt_.translate += velocity_;
}
