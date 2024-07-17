#include "TestHuman.h"



// 初期化処理
void TestHuman::Init()
{
	// Walk
	walkModel_ = make_unique<Model>();
	walkModel_->CreateGLTFModel("TestHuman", "walk", "uvChecker.png");
	walkAnim_ = AnimationManager::LoadAnimationFile("TestHuman", "walk");

	walkSkeleton_ = walkModel_->CreateSkeleton();
	walkSkinCluster_ = walkModel_->CreateSkinCluster(walkSkeleton_);

	walkWt_.Initialize();
	walkWt_.srt.translate.x = 2.0f;
	walkWt_.srt.rotate.y = ToRadians(180.0f);


	// Sneak
	sneakModel_ = make_unique<Model>();
	sneakModel_->CreateGLTFModel("TestHuman", "sneakWalk", "uvChecker.png");
	sneakAnim_ = AnimationManager::LoadAnimationFile("TestHuman", "sneakWalk");

	sneakSkeleton_ = sneakModel_->CreateSkeleton();
	sneakSkinCluster_ = sneakModel_->CreateSkinCluster(sneakSkeleton_);

	sneakWt_.Initialize();
	sneakWt_.srt.translate.x = -2.0f;
	sneakWt_.srt.rotate.y = ToRadians(180.0f);


	// SimpleSkin
	simpleModel_ = make_unique<Model>();
	simpleModel_->CreateGLTFModel("TestSimpleSkin", "simpleSkin", "simpleSkin.png");
	simpleAnim_ = AnimationManager::LoadAnimationFile("TestSimpleSkin", "simpleSkin");

	simpleSkeleton_ = simpleModel_->CreateSkeleton();
	simpleSkinCluster_ = simpleModel_->CreateSkinCluster(simpleSkeleton_);

	simpleWt_.Initialize();
	simpleWt_.srt.translate.x = 0.0f;
	simpleWt_.srt.rotate.y = ToRadians(180.0f);

}


// 更新処理
void TestHuman::Update()
{
	// Walk
	walkWt_.UpdateMatrix();

	// アニメーションの時間を進める
	animTimeA_ += 1.0f / 60.0f;
	if (animTimeA_ >= 1.0f) {
		animTimeA_ = 0.0f;
	}
	// アニメーションの更新を行って、骨ごとのLocal情報を更新する
	walkModel_->ApplyAnimation(walkSkeleton_, walkAnim_, animTimeA_);
	// 現在の骨ごとのLocal情報を基にSKeletonSpaceの情報を更新する
	walkModel_->UpdateSkeleton(walkSkeleton_);
	// SkeletonSpaceの情報を基にSkinClusterのMatrixPaletteを更新する
	walkModel_->UpdateSkinCluster(walkSkinCluster_, walkSkeleton_);


	// Sneak
	sneakWt_.UpdateMatrix();

	// アニメーションの時間を進める
	animTimeB_ += 0.3f / 60.0f;
	if (animTimeB_ >= 1.0f) {
		animTimeB_ = 0.0f;
	}
	// アニメーションの更新を行って、骨ごとのLocal情報を更新する
	sneakModel_->ApplyAnimation(sneakSkeleton_, sneakAnim_, animTimeB_);
	// 現在の骨ごとのLocal情報を基にSKeletonSpaceの情報を更新する
	sneakModel_->UpdateSkeleton(sneakSkeleton_);
	// SkeletonSpaceの情報を基にSkinClusterのMatrixPaletteを更新する
	sneakModel_->UpdateSkinCluster(sneakSkinCluster_, sneakSkeleton_);


	// SimpleSkin
	simpleWt_.UpdateMatrix();

	// アニメーションの時間を進める
	animTimeC_ += 0.5f / 60.0f;
	if (animTimeC_ >= 6.0f) {
		animTimeC_ = 0.0f;
	}
	// アニメーションの更新を行って、骨ごとのLocal情報を更新する
	simpleModel_->ApplyAnimation(simpleSkeleton_, simpleAnim_, animTimeC_);
	// 現在の骨ごとのLocal情報を基にSKeletonSpaceの情報を更新する
	simpleModel_->UpdateSkeleton(simpleSkeleton_);
	// SkeletonSpaceの情報を基にSkinClusterのMatrixPaletteを更新する
	simpleModel_->UpdateSkinCluster(simpleSkinCluster_, simpleSkeleton_);

#ifdef _DEBUG

	if (ImGui::TreeNode("TestHuman")) {

		ImGui::DragFloat3("Walk_Scale", &walkWt_.srt.scale.x, 0.1f);
		ImGui::DragFloat3("Walk_Rotate", &walkWt_.srt.rotate.x, 0.1f);
		ImGui::DragFloat3("Walk_Translate", &walkWt_.srt.translate.x, 0.1f);
		ImGui::DragFloat("Animationtime_A", &animTimeA_, 0.01f, 0.0f, 10.f);

		ImGui::Text("");
		ImGui::DragFloat3("Sneak_Scale", &sneakWt_.srt.scale.x, 0.1f);
		ImGui::DragFloat3("Sneak_Rotate", &sneakWt_.srt.rotate.x, 0.1f);
		ImGui::DragFloat3("Sneak_Translate", &sneakWt_.srt.translate.x, 0.1f);
		ImGui::DragFloat("Animationtime_B", &animTimeB_, 0.01f, 0.0f, 10.f);

		ImGui::Text("");
		ImGui::DragFloat3("Simple_Scale", &simpleWt_.srt.scale.x, 0.1f);
		ImGui::DragFloat3("Simple_Rotate", &simpleWt_.srt.rotate.x, 0.1f);
		ImGui::DragFloat3("Simple_Translate", &simpleWt_.srt.translate.x, 0.1f);
		ImGui::DragFloat("Animationtime_V", &animTimeC_, 0.01f, 0.0f, 10.f);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void TestHuman::Draw(Camera* camera)
{
	camera;
	// Walk
	walkModel_->AnimDraw(walkWt_, walkSkinCluster_, camera);

	// Sneak
	sneakModel_->AnimDraw(sneakWt_, sneakSkinCluster_, camera);

	// SimpleSkin
	simpleModel_->AnimDraw(simpleWt_, simpleSkinCluster_, camera);
}
