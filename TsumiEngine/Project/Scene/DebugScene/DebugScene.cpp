#include "DebugScene.h"



/// <summary>
/// コンストラクタ
/// </summary>
DebugScene::DebugScene() {}


/// <summary>
/// デストラクタ
/// </summary>
DebugScene::~DebugScene() {}


/// <summary>
/// 初期化処理
/// </summary>
void DebugScene::Initialize()
{
	/* ----- FileManager ファイルマネージャー ----- */
	FileManager::GetInstance()->LoadJsonFile("Json/", "honmei");

	/* ----- Camera カメラ ----- */
	camera_ = make_unique<Camera>();
	camera_->Initialize();
	camera_->rotate = { 0.2f, 0.0f, 0.0f };
	camera_->translate = { 0.0f, 5.0f, -15.0f };

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Initialize();

	/* ----- Skybox 天箱 ----- */
	//Skybox::GetInstance()->Initialize();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Initialize();

	/* ----- TestPostEffect テストポストエフェクト ----- */
	testPostEffect_ = make_unique<TestPostEffect>();
	testPostEffect_->Initialize();

	/* ----- TestHuman テストヒューマン ----- */
	testHuman_ = make_unique<TestHuman>();
	testHuman_->Init();
}


/// <summary>
/// 更新処理
/// </summary>
void DebugScene::Update(GameManager* state)
{
	state;

	/* ----- Camera カメラ ----- */
	camera_->UpdateMatrix();

	/* ----- Skydome 天球 ----- */
	Skydome::GetInstance()->Update();

	/* ----- Skybox 天箱 ----- */
	//Skybox::GetInstance()->Update();

	/* ----- Ground 床 ----- */
	Ground::GetInstance()->Update();

	/* ----- TestPostEffect テストポストエフェクト ----- */
	testPostEffect_->Update();

	/* ----- TestHuman テストヒューマン ----- */
	testHuman_->Update();

	if (Input::Trigger(DIK_RETURN)) {
		state->ChangeSceneState(new GameScene());
		return;
	}

#ifdef _DEBUG

	ImGui::Begin("GameScene");

	ImGui::Text("");
	ImGui::Text("Camera");
	ImGui::DragFloat3("Rotate", &camera_->rotate.x, 0.01f);
	ImGui::DragFloat3("Translate", &camera_->translate.x, 0.01f);
	ImGui::Text("");

	ImGui::End();

#endif // _DEBUG
}


/// <summary>
/// 背景スプライトの描画処理
/// </summary>
void DebugScene::BackSpriteDraw()
{

}


/// <summary>
/// ３Dオブジェクトの描画処理
/// </summary>
void DebugScene::ModelDraw()
{
	/* ----- Skydome 天球 ----- */
	//Skydome::GetInstance()->Draw(camera_.get());

	/* ----- Skybox 天箱 ----- */
	//Skybox::GetInstance()->Draw(camera_.get());

	/* ----- Ground 床 ----- */
	//Ground::GetInstance()->Draw(camera_.get());

	/* ----- TestHuman テストヒューマン ----- */
	testHuman_->Draw(camera_.get());
}


/// <summary>
/// 前景スプライトの描画処理
/// </summary>
void DebugScene::FrontSpriteDraw()
{
	/* ----- TestPostEffect テストポストエフェクト ----- */
	testPostEffect_->Draw(camera_.get());
}