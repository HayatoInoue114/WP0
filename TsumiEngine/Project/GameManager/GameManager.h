#pragma once

#include <chrono>

#include "../../Tsumi/Tsumi.h"
#include "../Scene/IScene.h"
#include "../Scene/DebugScene/DebugScene.h"
#include "../Scene/GameScene/GameScene.h"
#include "../../Tsumi/Utilities/TextLog/TextLog.h"

// グローバル変数の定義
extern float g_ElapsedTime;


/* GameManagerクラス */
class GameManager {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager(IScene* newScene);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

	/// <summary>
	/// ゲームループ
	/// </summary>
	void Run();

	/// <summary>
	/// シーンチェンジ
	/// </summary>
	void ChangeSceneState(IScene* newScene);


private:

	IScene* Scene_ = nullptr;

	std::chrono::steady_clock::time_point startTime_;
};