#include "Skydome.h"



// インスタンスの取得
Skydome* Skydome::GetInstance() 
{
	static Skydome instance;
	return &instance;
}


// 初期化処理
void Skydome::Initialize() 
{
	Skydome::GetInstance()->model_ = make_unique<Model>();
	Skydome::GetInstance()->model_->CreateFromObjAssimpVer("Skydome", "Skydome");
	Skydome::GetInstance()->worldTransform_.Initialize();
	Skydome::GetInstance()->worldTransform_.srt.scale = { 10000.0f, 1000.0f, 10000.0f };
}


// 更新処理
void Skydome::Update() 
{
	// ワールド座標の更新
	Skydome::GetInstance()->worldTransform_.UpdateMatrix();
}


// 描画処理
void Skydome::Draw(Camera* camera) 
{

	Skydome::GetInstance()->model_->Draw(Skydome::GetInstance()->worldTransform_, camera);
}
