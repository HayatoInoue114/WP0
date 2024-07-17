#include "TestAnimationCube.h"



// 初期化処理
void TestAnimationCube::Init()
{
	testModel_ = make_unique<Model>();
	testModel_->CreateGLTFModel("AnimatedCube", "AnimatedCube", "AnimatedCube.png");
	testAnimation_ = AnimationManager::LoadAnimationFile("AnimatedCube", "AnimatedCube");

	wt_.Initialize();
	wt_.srt.translate.y = 2.0f;
	wt_.srt.translate.z = 10.0f;
}


// 更新処理
void TestAnimationCube::Update()
{
	wt_.UpdateMatrix();

	// とりあえず時間の加算処理はここで行う
	animationtime += 1.0f / 60.0f;
	testModel_->PlayAnimation(testAnimation_, animationtime);

#ifdef _DEBUG

	if (ImGui::TreeNode("TestAnimationCube")) {

		ImGui::DragFloat3("Scale", &wt_.srt.scale.x, 0.1f);
		ImGui::DragFloat3("Rotate", &wt_.srt.rotate.x, 0.1f);
		ImGui::DragFloat3("Translate", &wt_.srt.translate.x, 0.1f);

		ImGui::Text("");
		ImGui::DragFloat("Animationtime", &animationtime, 0.01f, 0.0f, 10.f);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void TestAnimationCube::Draw(Camera* camera)
{
	testModel_->Draw(wt_, camera);
}

