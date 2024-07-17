#include "IEnemy.h"

void IEnemy::SpawnMove() {
	if (worldTransform_.translate().y <= 0) {
		worldTransform_.translate().y += 0.1f;
		worldTransform_.rotate().y += 0.1f;
		worldTransform_.UpdateMatrix();
	}
	else {
		isSpawn = true;
	}
}

void IEnemy::TitleMove()
{
	flame_++;

	Scope scope = { 420,720 };

	int jumpFlame = (int)RandomGenerator::getRandom(scope);

	if (flame_ % 120 == 0 && !isAnime_) {
		isAnime_ = true;
	}

	/*if (animationTime_ == 1.0f) {
		isAnime_ = false;
	}*/

	if (isAnime_) {
		// ジャンプの高さ
		float jumpHeight = 1.5f;

		// ジャンプの時間
		float jumpDuration = 1.0f;

		// 現在の時間
		float currentTime = 1.0f / 60.0f;

		animationTime_ += currentTime;

		if (animationTime_ == currentTime) {
			originalPos_ = worldTransform_.translate();
			originalScale_ = worldTransform_.scale();
		}

		worldTransform_.translate() = Jump(originalPos_, jumpHeight, jumpDuration, animationTime_);
		worldTransform_.scale() = JumpScale(originalScale_, jumpHeight, jumpDuration, animationTime_);
		Shake(originalPos_, jumpHeight, jumpDuration, animationTime_);

		worldTransform_.rotate().y += 0.1f;
	}
	/*else {
		animationTime_ = 0;
		worldTransform_.scale() = { (float)1,(float)1,(float)1 };
	}*/

	if (animationTime_ >= 1.0f) {
		if (worldTransform_.translate().y >= -50.0f) {
			worldTransform_.translate().y -= 0.1f;
			worldTransform_.rotate().y += 0.1f;
			worldTransform_.UpdateMatrix();
			isAnime_ = false;
		}
	}

	if (worldTransform_.translate().y <= -50.0f) {
		isAlive_ = false;
	}

	worldTransform_.UpdateMatrix();
}

Vector3 IEnemy::Jump(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime) {
	// ジャンプの高さが0以下なら初期位置を返す
	if (jumpHeight <= 0) {
		return initialPosition;
	}

	// 現在のジャンプ時間を[0, 1]の範囲に正規化する
	float normalizedTime = (float)min(currentTime / jumpDuration, 1.0f);

	Scope scope = { -0.1f,0.1f };
	ScopeVec3 vec3 = { scope,scope,scope };



	// ジャンプの進行度をイージング関数で計算する
	float jumpProgress;
	if (normalizedTime < 0.5f) {
		jumpProgress = jumpHeight * (float)Ease::OutQuint(normalizedTime * 2.0f);
		//worldTransform_.translate() += RandomGenerator::getRandom(vec3);
	}
	else {
		jumpProgress = jumpHeight * (1.0f - (float)Ease::OutQuint((normalizedTime - 0.5f) * 2.0f));
	}

	// 初期位置からの垂直方向の距離を計算し、それを元の位置に加える
	Vector3 finalPosition = initialPosition;
	finalPosition.y += jumpProgress;

	// ジャンプが終了したら元の位置に戻る
	if (currentTime >= jumpDuration) {
		finalPosition.y = initialPosition.y;
	}

	return finalPosition;
}

void IEnemy::Shake(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime) {

	// 現在のジャンプ時間を[0, 1]の範囲に正規化する
	float normalizedTime = (float)min(currentTime / jumpDuration, 1.0f);

	Scope scope = { -0.2f,0.2f };
	ScopeVec3 vec3 = { scope,scope,scope };

	// ジャンプの進行度をイージング関数で計算する
	float jumpProgress{};
	if (normalizedTime < 0.5f && flame_ % 5 == 0) {
		worldTransform_.translate() += RandomGenerator::getRandom(vec3);
		//worldTransform_.rotate() += RandomGenerator::getRandom(vec3);
	}
}

Vector3 IEnemy::JumpScale(const Vector3& initialScale, float jumpHeight, float jumpDuration, float currentTime) {
	// ジャンプの高さが0以下なら初期位置を返す
	if (jumpHeight <= 0) {
		return initialScale;
	}

	// ジャンプ後のスケールを計算
	Vector3 finalScale = initialScale;
	if (currentTime < jumpDuration) {
		finalScale += calculateJumpScale(currentTime * 1.1f, jumpDuration);
	}
	else {
		finalScale += calculateLandingScale(currentTime, jumpDuration, initialScale);
	}

	// ジャンプが終了したら元の位置に戻る
	if (currentTime >= jumpDuration) {
		finalScale = initialScale;
	}

	return finalScale;
}

// ジャンプ中のスケールを計算する関数
Vector3 IEnemy::calculateJumpScale(float currentTime, float jumpDuration) {
	// ジャンプ中の時間を[0, 1]の範囲に正規化する
	float normalizedTime = min(currentTime / jumpDuration, 1.0f);

	// スケールの計算
	float scaleMultiplier = 0.5f * (float)std::sin(M_PI * min(normalizedTime * 2.0f, 1.0f));
	return { -scaleMultiplier * 0.5f, scaleMultiplier, -scaleMultiplier * 0.5f };
}

// 着地時のスケールを計算する関数
Vector3 IEnemy::calculateLandingScale(float currentTime, float jumpDuration, const Vector3& initialScale) {
	// 着地時の時間を[0, 1]の範囲に正規化する
	float normalizedTime = min(currentTime / jumpDuration, 1.0f);
	// イージング関数を使用してスケールの変化を計算
	float scaleMultiplier = 1.0f - (float)Ease::OutQuint(normalizedTime);

	// xとz方向のスケールを増やし、y方向のスケールを減らす
	Vector3 landingScale = {
		initialScale.x + scaleMultiplier * 0.5f, // x方向のスケールを増やす
		initialScale.y * (1.0f - scaleMultiplier), // y方向のスケールを減らす
		initialScale.z + scaleMultiplier * 0.5f // z方向のスケールを増やす
	};

	return landingScale;
}

float IEnemy::CalculateAngle(const Vector3& current, const Vector3& target) {
	// 目標位置と現在位置の差を計算
	float deltaX = target.x - current.x;
	float deltaZ = target.z - current.z;

	// atan2関数を使用して角度を計算
	return std::atan2(deltaX, deltaZ);
}