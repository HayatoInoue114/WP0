#include "GameManager.h"


// グローバル変数の定義
float g_ElapsedTime = 0.0f;

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager(IScene* newScene) {

	Tsumi::Initialize();
	Scene_ = newScene;
	Scene_->Initialize();
	startTime_ = std::chrono::steady_clock::now();  // 開始時間を記録
}


/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager() {

	delete Scene_;
	Tsumi::Finalize();
}


/// <summary>
/// ゲームループ
/// </summary>
void GameManager::Run() {

	TextLog::Init();
	TextLog::Load("Run_Start");
	while (Tsumi::ProcessMessage() == 0) {

		auto currentTime = std::chrono::steady_clock::now();
		g_ElapsedTime = std::chrono::duration<float>(currentTime - startTime_).count();

		Tsumi::BeginFlame();
		Scene_->Update(this);

		//// ポストエフェクト
		DirectXCommon::PreDrawForPostEffect();
		
		Scene_->BackSpriteDraw();
		Scene_->ModelDraw();

		DirectXCommon::PostDrawForPostEffect();

		// スワップチェーン
		DirectXCommon::PreDrawForSwapChain();

		Scene_->FrontSpriteDraw();

		Tsumi::EndFlame();
		DirectXCommon::PostDrawForSwapChain();

		// 経過時間の表示（デバッグ用）
		std::cout << "Elapsed Time: " << g_ElapsedTime << " seconds" << std::endl;
	}
	TextLog::Load("Run_End");
	TextLog::Close();
}


/// <summary>
/// シーンチェンジ
/// </summary>
void GameManager::ChangeSceneState(IScene* newScene) {

	delete Scene_;
	Scene_ = newScene;
	Scene_->Initialize();
	return;
}