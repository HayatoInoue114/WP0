#include "Enemy.h"
#include "../../Player/Player.h"


// 初期化処理
void Enemy::Initialize()
{
	// BodyModelのロードと初期化
	bodyModel_ = make_unique<Model>();
	bodyModel_->CreateFromObjAssimpVer("Test", "Test");

	// BodyTransformの初期化
	bodyWt_.Initialize();
	// 0.0fだと行列計算でエラーが発生。限りなく0に近い数字で0.1f。
	bodyWt_.srt.scale = { 0.1f, 0.1f, 0.1f }; 

	// ShotFrameにIntervalを入れておく
	shotFrame_ = kShotInterval_;

	/* ----- StatePattern ステートパターン ----- */
	// 各ステートをコンテナに保存
	stateVector_.resize(EnumSize<EnemyState>::value);
	stateVector_[EnemyState::SPAWN] = std::make_unique<IEnemySpawnState>();
	stateVector_[EnemyState::APPROACH] = std::make_unique<IEnemyApproachState>();
	stateVector_[EnemyState::DEATH] = std::make_unique<IEnemyDeathState>();
	// 初期ステートの設定 && 初期ステートの初期化処理
	stateNo_ = EnemyState::SPAWN;
	currentStateNo_ = stateNo_;
	stateVector_[currentStateNo_]->Init(this);
}


// 更新処理
void Enemy::Update()
{
	// ステートパターン処理
	FuncStatePattern();

	// Transformの更新処理
	bodyWt_.UpdateMatrix();

	// アプローチ状態の時のみ入る処理
	if (stateNo_ == EnemyState::APPROACH) {

		// 戦闘状態の切り替え処理
		ToggleCombatState();

		// 戦闘状態に入っていたら入る処理
		if (isCombatActive_) {

			// 移動処理
			Move();

			// 射撃の処理
			ExecuteShot();
		}
	}

	// Bullet更新処理
	for (std::shared_ptr<EnemyBullet> bullet : bulletList_) {
		bullet->Update();
	}

	// 死亡フラグが立っていたら削除
	bulletList_.remove_if([](std::shared_ptr<EnemyBullet> bullet) {
		if (bullet->IsDead()) {
			bullet.reset();
			return true;
		}
		return false;
		}
	);


#ifdef _DEBUG

	/*if (ImGui::TreeNode("Enemy")) {

		ImGui::Text("Transform");
		ImGui::DragFloat3("Scale", &bodyWt_.srt.scale.x, 0.01f, 0.0f, 20.0f);
		ImGui::DragFloat3("Rotate", &bodyWt_.srt.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &bodyWt_.srt.translate.x, 0.01f);

		ImGui::Text("");
		ImGui::Checkbox("CombatActive", &isCombatActive_);

		ImGui::Text("");
		ImGui::Text("ShotFrame = %d : Interval = %d", shotFrame_, kShotInterval_);

		ImGui::TreePop();
	}*/

#endif // _DEBUG
}


// 描画処理
void Enemy::Draw3D(Camera* camera)
{
	// BodyModelの描画
	bodyModel_->SetColor(modelColor_);
	bodyModel_->Draw(bodyWt_, camera);

	// Bulletsの描画
	for (std::shared_ptr<EnemyBullet> bullet : bulletList_) {
		bullet->Draw3D(camera);
	}
}


// 衝突自コールバック関数
void Enemy::OnCollisionWithPlayer()
{

}
void Enemy::OnCollisionWithPlayerBullet()
{
	// スポーン&デス時には通らない
	if (stateNo_ != EnemyState::SPAWN && stateNo_ != EnemyState::DEATH) {

		// デスステートに移行
		this->ChangeState(EnemyState::DEATH);
	}
}


// ステートパターン処理
void Enemy::FuncStatePattern()
{
	// ステートチェック
	preStateNo_ = currentStateNo_;
	currentStateNo_ = stateNo_;

	// ステート変更チェック
	if (preStateNo_ != currentStateNo_) {

		///// 前回のステートの終了処理
		stateVector_[preStateNo_]->Exit();

		///// 新しいステートの初期化処理
		stateVector_[currentStateNo_]->Init(this);
	}

	///// 更新処理
	stateVector_[currentStateNo_]->Update();
}


// 戦闘状態の切り替え処理
void Enemy::ToggleCombatState()
{
	// プレイヤーとの距離で戦闘状態のフラグを管理する
	// 設定した距離よりも近くにいたらフラグを立てる
	if (std::abs(Length(player_->GetPosition() - bodyWt_.GetWorldPos())) <= combatTriggerDistance_) {

		// 戦闘状態のフラグを立てる
		isCombatActive_ = true;
	}
	else {

		// 戦闘状態のフラグを折る
		isCombatActive_ = false;

		// 射撃までのフレームを設定
		shotFrame_ = kShotInterval_;
	}
}


// 移動処理
void Enemy::Move()
{
	// ある程度近ければ早期return
	if (std::abs(Length(player_->GetPosition() - bodyWt_.GetWorldPos())) <= minToPlayer_) {
		return;
	}

	// velocityを計算
	CalcVelocity();

	// 姿勢の計算
	CalcRotate();

	// 座標にvelocityを加算
	bodyWt_.srt.translate += velocity_;
}


// Velocityの計算処理
void Enemy::CalcVelocity()
{
	// 差分をNormalize
	Vector3 player2Enemy =
		Normalize(player_->GetPosition() - bodyWt_.GetWorldPos());

	// 差分Normalizeに速度をかけてvelocityに設定
	velocity_ = {
		player2Enemy.x * moveVector_,
		player2Enemy.y, // y軸は移動してほしくないのでそのまま
		player2Enemy.z * moveVector_,
	};
}


// 向きの計算処理
void Enemy::CalcRotate()
{
	// Y軸周り角度(θy)
	bodyWt_.srt.rotate.y = std::atan2(velocity_.x, velocity_.z);

	float velZ = std::sqrt((velocity_.x * velocity_.x) + (velocity_.z * velocity_.z));
	float height = -velocity_.y;

	// X軸周り角度(θx)
	bodyWt_.srt.rotate.x = std::atan2(height, velZ);
}


// 射撃の処理
void Enemy::ExecuteShot()
{
	// タイマーをデクリメント
	shotFrame_--;

	// 0以下になったら射撃&タイマーリセット
	if (shotFrame_ <= 0) {

		// バレット生成
		CreateNewBullet();

		// タイマー再設定
		shotFrame_ = kShotInterval_;
	}
}


// 新しいバレットを生成する
void Enemy::CreateNewBullet()
{
	// newBulletのインスタンス
	std::shared_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();

	// 初期座標
	Vector3 initPos = bodyWt_.GetWorldPos();
	// 初期速度
	Vector3 initVel = Vector3::oneZ;
	initVel.z = 0.1f;
	initVel = TransformNormal(initVel, bodyWt_.matWorld);

	// newBulletの初期化
	newBullet->Initialize();
	newBullet->SetPosition(initPos);
	newBullet->SetVelocity(initVel);
	newBullet->SetRotationFromVelocity();

	// リストに追加
	bulletList_.push_back(newBullet);
}

