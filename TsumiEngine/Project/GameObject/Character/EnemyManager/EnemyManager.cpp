#include "EnemyManager.h"
#include "../Player/Player.h"


// 初期化処理
void EnemyManager::Initialize()
{
	// FlagModel
	flagModel_ = make_unique<Model>();
	flagModel_->CreateFromObjAssimpVer("Flag", "Flag");

	// Transformの初期化
	transform_.Initialize();
	transform_.srt.translate.z = 30.0f; // 少し奥にずらしておく

	// 湧き範囲のスコープ
	scope3_ = {
		{ -12.0f, 12.0f },
		{  0.0f, 0.0f },
		{ -12.0f, 12.0f },
	};

	// エネミーの最低数の設定
	enemyMinInstance_ = 5;

	// エネミーのカウントチェックタイマーの設定。4秒
	enemyCountCheckTime_.Start(0.0f, 240.0f);
}


// 更新処理
void EnemyManager::Update()
{
	// Transformの更新処理
	transform_.UpdateMatrix();

	// EnemyListの更新処理
	for (std::shared_ptr<IEnemy> enemy : enemyList_) {
		enemy->Update();
	}

	// 死亡フラグが立っていたら削除
	enemyList_.remove_if([](std::shared_ptr<IEnemy> enemy) {
		if (enemy->IsDead()) {
			enemy.reset();
			return true;
		}
		return false;
		}
	);

	// エネミーカウントチェック
	EnemyCountCheck();

#ifdef _DEBUG
	if (ImGui::TreeNode("EnemyManager")) {

		ImGui::Text("Transform");
		ImGui::DragFloat3("Scale", &transform_.srt.scale.x, 0.01f, 0.0f, 20.0f);
		ImGui::DragFloat3("Rotate", &transform_.srt.rotate.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.srt.translate.x, 0.1f);

		ImGui::Text("");
		if (ImGui::Button("AddEnemy")) {
			AddNewEnemy();
		}
		ImGui::Text("EnemyInstance = %d", int(enemyList_.size()));
		ImGui::Text("CountCheckTime : %.1f / %.1f", enemyCountCheckTime_.GetNowFrame(), enemyCountCheckTime_.GetEndFrame());

		ImGui::TreePop();
	}
#endif // _DEBUG
}


// 描画処理
void EnemyManager::Draw3D(Camera* camera)
{
	// FlagModel
	flagModel_->Draw(transform_, camera);

	// EnemyListの描画
	for (std::shared_ptr<IEnemy> enemy : enemyList_) {
		enemy->Draw3D(camera);
	}
}


// 新しいEnemyを追加する
void EnemyManager::AddNewEnemy()
{
	CreateNewEnemy(); // 新しいEnemyを生成する
}


// 新しいEnemyを生成する
void EnemyManager::CreateNewEnemy()
{
	// 新しいEnemyのインスタンス
	std::shared_ptr<IEnemy> newEnemy = std::make_unique<IEnemy>();

	// 初期座標。多少ランダムに湧く
	Vector3 initPos = 
		transform_.GetWorldPos() + RandomGenerator::getRandom(scope3_);

	// newEnemyの初期化
	newEnemy->Initialize();
	newEnemy->SetPlayer(player_);
	newEnemy->SetPosition(initPos);

	// リストに追加
	enemyList_.push_back(newEnemy);
}


// エネミーカウントチェック
void EnemyManager::EnemyCountCheck()
{
	// タイマー更新
	enemyCountCheckTime_.Update(true);

	// タイマー終了でチェック
	if (enemyCountCheckTime_.IsFinish()) {

		// エネミーが一定数以下なら新しく湧くようにする
		if (enemyMinInstance_ >= int(enemyList_.size())) {

			// 最低数との差分
			int shortageCount = enemyMinInstance_ - int(enemyList_.size());

			// 足りない分、新しいEnemyを生成する
			for (int i = 0; i < shortageCount; ++i) {
				CreateNewEnemy();
			}
		}
	}
}
