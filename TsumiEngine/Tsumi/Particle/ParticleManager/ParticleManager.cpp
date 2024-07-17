#include "ParticleManager.h"



// 初期化処理
void ParticleManager::Initialize()
{
	// パーティクルの配列
	particleArray_ = make_unique<ParticleArray>();

	// 描画クラスの初期化
	renderer_ = make_unique<ParticleRenderer>();
	renderer_->Initialize(2048u);
}


// 更新処理
void ParticleManager::Update()
{
	particleArray_->Update();
}


// 描画処理
void ParticleManager::Draw(Camera* camera)
{
	const auto& particle = particleArray_->GetParticleArray();
	renderer_->Draw(particle, camera);
}


// パーティクルの追加
void ParticleManager::AddParticle(unique_ptr<IParticle> particle)
{
	particleArray_->push_back(move(particle));
}

