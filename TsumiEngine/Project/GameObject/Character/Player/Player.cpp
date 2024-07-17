#include "Player.h"



// 初期化処理
void Player::Initialize()
{
	// Inputクラス
	input_ = Input::GetInstance();

	// BodyModelのロードと初期化
	bodyModel_ = make_unique<Model>();
	bodyModel_->CreateFromObjAssimpVer("Player", "Player");

	// BodyTransformの初期化
	bodyWt_.Initialize();
}


// 更新処理
void Player::Update()
{
	// Transformの更新処理
	bodyWt_.UpdateMatrix();

	// 移動処理
	Move();

	// プレイヤー本体の姿勢処理
	CalcBodyRotate();

	// 射撃の処理
	ExecuteShot();

	// Bulletの更新処理
	for (std::shared_ptr<PlayerBullet> bullet : bulletList_) {
		bullet->Update();
	}

	// 死亡フラグが立っていたら削除
	bulletList_.remove_if([](std::shared_ptr<PlayerBullet> bullet) {
		if (bullet->IsDead()) {
			bullet.reset();
			return true;
		}
		return false;
		}
	);

#ifdef _DEBUG

	if (ImGui::TreeNode("Player")) {

		ImGui::DragFloat3("Scale", &bodyWt_.srt.scale.x, 0.01f, 0.0f, 20.0f);
		ImGui::DragFloat3("Rotate", &bodyWt_.srt.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &bodyWt_.srt.translate.x, 0.01f);

		ImGui::Text("");
		ImGui::Text("ShotFrame = %d", shotPressFrame_);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void Player::Draw2DBack(Camera* camera)
{
	camera;
}
void Player::Draw3D(Camera* camera)
{
	// BodyModelの描画
	bodyModel_->Draw(bodyWt_, camera);

	// Bulletsの描画
	for (std::shared_ptr<PlayerBullet> bullet : bulletList_) {
		bullet->Draw3D(camera);
	}
}
void Player::Draw2DFront(Camera* camera)
{
	camera;
}


// 衝突自コールバック関数
void Player::OnCollisionWithEnemy()
{


}
void Player::OnCollisionWithEnemyBullet()
{


}


// 移動処理
void Player::Move()
{
	// velocityは0で毎フレーム初期化
	velocity_ = Vector3::zero;

	// キーの処理
	if (input_->Press(DIK_W))
	{
		velocity_.z = moveVector_;
	};
	if (input_->Press(DIK_A))
	{
		velocity_.x = -moveVector_;
	};
	if (input_->Press(DIK_S))
	{
		velocity_.z = -moveVector_;
	};
	if (input_->Press(DIK_D))
	{
		velocity_.x = moveVector_;
	};

	// パッドの処理
	if (input_->GetLStick().x <= -0.3f)
	{
		velocity_.x = -1.0f;
	}
	if (input_->GetLStick().x >= 0.3f)
	{
		velocity_.x = 1.0f;
	}
	if (input_->GetLStick().y <= -0.3f)
	{
		velocity_.z = -1.0f;
	}
	if (input_->GetLStick().y >= 0.3f)
	{
		velocity_.z = 1.0f;
	}

	// 正規化
	if (velocity_.x != 0.0f || velocity_.z != 0.0f) {
		float length = Length({ velocity_.x, velocity_.z });
		velocity_.x /= length;
		velocity_.z /= length;
	}

	// velocityに速度を掛けて座標に加算
	bodyWt_.srt.translate += (velocity_ * moveVector_);
}


// プレイヤー本体の姿勢処理
void Player::CalcBodyRotate()
{
	// 射撃時はRStickのInputを取得
	if (input_->Press(DIK_SPACE) || input_->Press(PadData::RIGHT_SHOULDER)) {
		
		//Stickの入力を取得
		stickInput_ = input_->GetRStick();
	}
	else {
		//Stickの入力を取得
		stickInput_ = input_->GetLStick();
	}

	// Stick入力がいていい範囲を超えている場合、角度を更新
	// stick入力が一定範囲を超えている場合、角度を更新
	if (std::abs(stickInput_.x) > 0.2f || std::abs(stickInput_.y) > 0.2f) {

		// Y軸周り角度(θy)
		bodyWt_.srt.rotate.y = std::atan2(stickInput_.x, stickInput_.y);
	}
}


// 射撃処理
void Player::ExecuteShot()
{
	// キーorボタン押下でタイマーをデクリメント
	if (input_->Press(DIK_SPACE) || input_->Press(PadData::RIGHT_SHOULDER)) {

		shotPressFrame_--;

		if (shotPressFrame_ <= 0) {
			// 0以下でタイマー再設定
			shotPressFrame_ = kShotInterval_;
			// バレット生成
			CreateNewBullet();
		}
	}
	// キーorボタンを離したら、最初の1発目がすぐ出るように1フレームを入れておく
	else if (input_->Release(DIK_SPACE) || input_->Release(PadData::RIGHT_SHOULDER)) {
		
		shotPressFrame_ = 1;
	}
}


// 新しいバレットを生成する
void Player::CreateNewBullet()
{
	// newBulletのインスタンス
	std::shared_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

	// 初期座標
	Vector3 initPos = bodyWt_.GetWorldPos();
	// 初期速度
	Vector3 initVel = Vector3::oneZ;
	initVel = TransformNormal(initVel, bodyWt_.matWorld);

	// newBulletの初期化
	newBullet->Initialize();
	newBullet->SetPosition(initPos);
	newBullet->SetVelocity(initVel);
	newBullet->SetRotationFromVelocity();

	// リストに追加
	bulletList_.push_back(newBullet);
}

