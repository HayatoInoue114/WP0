#include "EnemyBullet.h"



// 初期化処理
void EnemyBullet::Initialize()
{
	// Modelの初期化
	bulletModel_ = std::make_unique<Model>();
	bulletModel_->CreateFromObjAssimpVer("DemoBullet", "DemoBullet");

	// Transformの初期化。座標や姿勢の設定は呼び出し先でaccessorで設定
	bulletWt_.Initialize();

	// 速度の設定。呼び出し先でaccessorで設定

	// 寿命のタイマーをスタート。2秒で設定
	life_.Start(0.0f, 5.0f * 60.0f);
}


// 更新処理
void EnemyBullet::Update()
{
	// Transformの更新処理
	bulletWt_.UpdateMatrix();

	// 移動処理
	Move();

	// 寿命の処理
	RemoveAfterlifeTime();
}


// 描画処理
void EnemyBullet::Draw3D(Camera* camera)
{
	bulletModel_->Draw(bulletWt_, camera);
}


// 衝突自コールバック関数
void EnemyBullet::OnCollisionWithPlayer()
{
	isDead_ = true;
}
void EnemyBullet::OnCollisionWithPlayerBullet()
{
	isDead_ = true;
}


// 移動処理
void EnemyBullet::Move()
{
	bulletWt_.srt.translate += velocity_;
}


// 寿命の処理
void EnemyBullet::RemoveAfterlifeTime()
{
	// 寿命の更新
	life_.Update();

	// タイマーが規定値になったら
	if (life_.IsFinish()) {

		// 死亡フラグを立てる
		isDead_ = true;

		// 寿命のタイマーをクリアしとく
		life_.Clear();
	}
}
