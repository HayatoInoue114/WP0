#pragma once

#include "../../Project/Math/MyMath.h"


// Particle
struct ParticleProperties {
	WorldTransform worldTransform;
	WorldTransform uvTransform;
	Vector4 color;
	Vector3 velocity;
	uint32_t lifeTime;
	uint32_t currentTime;
	bool isActive;
};


/* IParticleクラス */
class IParticle {

public: // メンバ関数
	 
	// コンストラクタ、デストラクタ
	IParticle() {};
	virtual ~IParticle() {};

	// 初期化処理、更新処理、描画処理
	virtual void Initialize() = 0;
	virtual void Update() = 0;


#pragma region Get 取得

	// トランスフォームの取得
	WorldTransform GetTransform() { return this->wt_; }

	// UVTransformの取得
	WorldTransform GetUvTransform() { return this->uv_; }

	// 発生率の取得
	float GetRate() { return this->perSecond_; }

	// 発生方向の取得
	Vector3 GetDirection() { return this->direction_; }

	// 初期速度の取得
	Vector3 GetVelocity() { return this->velocity_; }

	// 寿命の取得
	float GetLifeTime() { return this->lifeTime_; }
	float GetMinLifeTime() { return this->minLifetime_; }
	float GetMaxLifeTime() { return this->maxLifetime_; }

	// 色の取得
	Vector4 GetColor() { return this->color_; }

	// アクティブフラグ
	bool GetIsActive() { return this->isActive_; }

#pragma endregion 

#pragma region Set 設定

	// トランスフォームの取得
	void SetTransform(WorldTransform setWt) { this->wt_ = setWt; }

	// UVTransformの取得
	void SetUvTransform(WorldTransform setUv) { this->uv_ = setUv; }

	// 発生率の設定
	void SetRate(float setPs) { this->perSecond_ = setPs; }

	// 発生方向の設定
	void SetDirection(Vector3 setDirection) { this->direction_ = setDirection; }

	// 初期速度の設定
	void SetVelocity(Vector3 setVel) { this->velocity_ = setVel; }

	// 寿命の設定
	void SetLifeTime(float setTime) { this->lifeTime_ = setTime; }
	void SetMinMaxLifeTime(float setMin, float setMax) { this->minLifetime_ = setMin; this->maxLifetime_ = setMax; }
	void SetMinLifeTime(float setMin) { this->minLifetime_ = setMin; }
	void SetMaxLifeTime(float setMax) { this->maxLifetime_ = setMax; }

	// 色の設定
	void SetColor(Vector4 setColor) { this->color_ = setColor; }

	// アクティブフラグ
	void SetIsActive(bool state) { this->isActive_ = state; }

#pragma endregion 

protected: // メンバ変数

	// トランスフォーム
	WorldTransform wt_{};

	// uv
	WorldTransform uv_{};

	// 発生率
	float perSecond_;

	// 発生方向
	Vector3 direction_{};

	// 初期速度
	Vector3 velocity_{};

	// 寿命
	float lifeTime_;
	float minLifetime_;
	float maxLifetime_;

	// 色
	Vector4 color_{};

	// アクティブフラグ
	bool isActive_;
};