#pragma once

#include "../ParticleRenderer/ParticleRenderer.h"
#include "../ParticleArray/ParticleArray.h"

/* ParticleManagerクラス */
class ParticleManager {

private: // シングルトンパターン

	// コンストラクタ、デストラクタ
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

public: // メンバ関数

	// インスタンスの取得
	static ParticleManager* GetInstance() {
		static ParticleManager instance;
		return &instance;
	}

	// 初期化処理、更新処理、描画処理
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	// パーティクルの追加
	void AddParticle(unique_ptr<IParticle> particle);

private: // メンバ関数


private: // メンバ変数

	// パーティクルの配列
	unique_ptr<ParticleArray> particleArray_ = nullptr;

	// 描画クラス
	unique_ptr<ParticleRenderer> renderer_ = nullptr;

};

