#include "TestPostEffect.h"



// 初期化処理
void TestPostEffect::Initialize()
{
	// Absent
	absent_ = std::make_unique<AbsentEffect>();
	absent_->Initialize();

	// BoxFilter
	boxFilter_ = std::make_unique<BoxFilterEffect>();
	boxFilter_->Initialize();

	// ColorGrading
	colorGrading_ = std::make_unique<ColorGradingEffect>();
	colorGrading_->Initialize();

	// Dissolve
	dissolve_ = std::make_unique<DissolveEffect>();
	dissolve_->Initialize();

	// GaussianFilter
	gaussianFilter_ = std::make_unique<GaussianFilterEffect>();
	gaussianFilter_->Initialize();

	// Grain
	grain_ = std::make_unique<GrainEffect>();
	grain_->Initialize();

	// LuminanceOutLine
	luminanceOutLine_ = std::make_unique<LuminanceOutLineEffect>();
	luminanceOutLine_->Initialize();

	// RadialBlur
	radialBlur_ = std::make_unique<RadialBlurEffect>();
	radialBlur_->Initialize();

	// Random
	random_ = std::make_unique<RandomEffect>();
	random_->Initialize();

	// SepiaTone
	sepiaTone_ = std::make_unique<SepiaToneEffect>();
	sepiaTone_->Initialize();

	// Vignetting
	vignetting_ = std::make_unique<VignettingEffect>();
	vignetting_->Initialize();


	// IPostEffect::Typeを表す文字
	imguiEffectName_ = {
		"Absent",
		"BoxFilter",
		"ColorGrading",
		"Dissolve",
		"GaussianFilter",
		"Grain",
		"GrayScale",
		"LuminanceOutLine",
		"RadialBlur",
		"Random",
		"SepiaTone",
		"Vignetting"
	};

	activeEffects_ = EffectType::Absent;
}


// 更新処理
void TestPostEffect::Update()
{
#ifdef _DEBUG

	if (ImGui::TreeNode("TestPostEffect")) {

		ChangeImGui();

		if (activeEffects_ == EffectType::Absent) {
			absent_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::BoxFilter) {
			boxFilter_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::ColorGrading) {
			colorGrading_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Dissolve) {
			dissolve_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::GauusianFilter) {
			gaussianFilter_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Grain) {
			grain_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::LuminanceOutLine) {
			luminanceOutLine_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::RadialBlur) {
			radialBlur_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Random) {
			random_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::SepiaTone) {
			sepiaTone_->DrawImGui();
		}
		else if (activeEffects_ == EffectType::Vignetting) {
			vignetting_->DrawImGui();
		}
		ImGui::Text("");

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void TestPostEffect::Draw(Camera* camera)
{
	camera;
	if (activeEffects_ == EffectType::Absent) {
		absent_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::BoxFilter) {
		boxFilter_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::ColorGrading) {
		colorGrading_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::Dissolve) {
		dissolve_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::GauusianFilter) {
		gaussianFilter_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::Grain) {
		grain_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::LuminanceOutLine) {
		luminanceOutLine_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::RadialBlur) {
		radialBlur_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::Random) {
		random_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::SepiaTone) {
		sepiaTone_->Draw(camera);
	}
	else if (activeEffects_ == EffectType::Vignetting) {
		vignetting_->Draw(camera);
	}

}


// ImGuiの描画
void TestPostEffect::ChangeImGui()
{
	// std::vector<std::string>をstd::vector<const char*>に変換
	std::vector<const char*> cstrEffectNames;
	for (const auto& name : imguiEffectName_) {
		cstrEffectNames.push_back(name.c_str());
	}

	if (ImGui::Combo("EffectType", &imguiComboIndex_, cstrEffectNames.data(), int(cstrEffectNames.size()))) {

		switch (imguiComboIndex_) {
		case 0: activeEffects_ = EffectType::Absent; break;
		case 1: activeEffects_ = EffectType::BoxFilter; break;
		case 2: activeEffects_ = EffectType::ColorGrading; break;
		case 3: activeEffects_ = EffectType::Dissolve; break;
		case 4: activeEffects_ = EffectType::GauusianFilter; break;
		case 5: activeEffects_ = EffectType::Grain; break;
		case 6: activeEffects_ = EffectType::GrayScale; break;
		case 7: activeEffects_ = EffectType::LuminanceOutLine; break;
		case 8: activeEffects_ = EffectType::RadialBlur; break;
		case 9: activeEffects_ = EffectType::Random; break;
		case 10: activeEffects_ = EffectType::SepiaTone; break;
		case 11: activeEffects_ = EffectType::Vignetting; break;
		}
	}

}
