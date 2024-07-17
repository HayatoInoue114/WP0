#include "TestJsonObject.h"



// 初期化処理
void TestJsonObject::Initialize()
{
	// テストモデル : 壁
	wallModel_ = make_unique<Model>();
	wallModel_->CreateFromObjAssimpVer("ShamWall", "ShamWall");
	wallWt_.Initialize();
	wallWt_.srt = FileManager::GetInstance()->GetObjectSrt("ShamWall");


	// テストモデル : 棒
	bouModel_ = make_unique<Model>();
	bouModel_->CreateFromObjAssimpVer("Bou", "Bou");
	bouWt_.Initialize();
	bouWt_.srt = FileManager::GetInstance()->GetObjectSrt("Bou");


	// テストモデル : 具
	guModel_ = make_unique<Model>();
	guModel_->CreateFromObjAssimpVer("Gu", "Gu");
	guWt_.Initialize();
	guWt_.srt = FileManager::GetInstance()->GetObjectSrt("Gu");
}


// 更新処理
void TestJsonObject::Update()
{
	// テストモデル : 壁
	wallWt_.UpdateMatrix();

	// テストモデル : 棒
	bouWt_.UpdateMatrix();

	// テストモデル : 具
	guWt_.UpdateMatrix();
}


// 描画処理
void TestJsonObject::Draw(Camera* camera)
{
	// テストモデル : 壁
	//wallModel_->Draw(wallWt_, camera);

	// テストモデル : 棒
	bouModel_->Draw(bouWt_, camera);

	// テストモデル : 具
	guModel_->Draw(guWt_, camera);

}
