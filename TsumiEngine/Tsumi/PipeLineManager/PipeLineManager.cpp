#include "PipeLineManager.h"


// PipeLineType
PipeLineType PipeLineManager::nowPipeLineType_ = PipeLineType::None;


// 初期化処理
void PipeLineManager::Initialize()
{
	// インスタンスの取得
	PipeLineManager* instance = PipeLineManager::GetInstance();


	// Lambert
	instance->lambertPipeLine_ = make_unique<LambertPipeLine>();
	instance->pipeLineMap_[PipeLineType::Lambert] = instance->lambertPipeLine_->SetUpPso();

	// Light
	instance->lightPipeLine_ = make_unique<LightPipeLine>();
	instance->pipeLineMap_[PipeLineType::Light] = instance->lightPipeLine_->SetUpPso();

	// Line
	instance->linePipeLine_ = make_unique<LinePipeLine>();
	instance->pipeLineMap_[PipeLineType::Line] = instance->linePipeLine_->SetUpPso();

	// Object3D
	instance->object3DPipeLine_ = make_unique<Object3DPipeLine>();
	instance->pipeLineMap_[PipeLineType::Object3D] = instance->object3DPipeLine_->SetUpPso();

	// Particle
	instance->particlePipeLine_ = make_unique<ParticlePipeLine>();
	instance->pipeLineMap_[PipeLineType::Particle] = instance->particlePipeLine_->SetUpPso();

	// PhongNormalMap
	instance->phongNormalMapPipeLine_ = make_unique<PhongNormalMapPipeLine>();
	instance->pipeLineMap_[PipeLineType::PhongNormalMap] = instance->phongNormalMapPipeLine_->SetUpPso();

	// Phong
	instance->phongPipeLine_ = make_unique<PhongPipeLine>();
	instance->pipeLineMap_[PipeLineType::Phong] = instance->phongPipeLine_->SetUpPso();

	// SkinningObject3d
	instance->skinningObject3DPipeLine_ = make_unique<SkinningObject3dPipeLine>();
	instance->pipeLineMap_[PipeLineType::SkinningObject3D] = instance->skinningObject3DPipeLine_->SetUpPso();

	// Skybox
	instance->skyboxPipeLine_ = make_unique<SkyboxPipeLine>();
	instance->pipeLineMap_[PipeLineType::Skybox] = instance->skyboxPipeLine_->SetUpPso();

	// Sprite
	instance->spritePipeLine_ = make_unique<SpritePipeLine>();
	instance->pipeLineMap_[PipeLineType::Sprite] = instance->spritePipeLine_->SetUpPso();


	// Absent
	instance->absentPipeLine_ = make_unique<AbsentPipeLine>();
	instance->pipeLineMap_[PipeLineType::Absent] = instance->absentPipeLine_->SetUpPso();

	// BoxFilter
	instance->boxFilterPipeLine_ = make_unique<BoxFilterPipeLine>();
	instance->pipeLineMap_[PipeLineType::BoxFilter] = instance->boxFilterPipeLine_->SetUpPso();

	// Dissolve
	instance->dissolvePipeLine_ = make_unique<DissolvePipeLine>();
	instance->pipeLineMap_[PipeLineType::Dissolve] = instance->dissolvePipeLine_->SetUpPso();

	// GaussianFilter
	instance->gaussianFilterPipeLine_ = make_unique<GaussianFilterPipeLine>();
	instance->pipeLineMap_[PipeLineType::GaussianFilter] = instance->gaussianFilterPipeLine_->SetUpPso();

	// Grain
	instance->grainPipeLine_ = make_unique<GrainPipeLine>();
	instance->pipeLineMap_[PipeLineType::Grain] = instance->grainPipeLine_->SetUpPso();

	// GrayScale
	instance->grayScalePipeLine_ = make_unique<GrayScalePipeLine>();
	instance->pipeLineMap_[PipeLineType::GrayScale] = instance->grayScalePipeLine_->SetUpPso();

	// OutLine
	instance->outLinePipeLine_ = make_unique<OutLinePipeLine>();
	instance->pipeLineMap_[PipeLineType::OutLine] = instance->outLinePipeLine_->SetUpPso();

	// RadialBlur
	instance->radialBlurPipeLine_ = make_unique<RadialBlurPipeLine>();
	instance->pipeLineMap_[PipeLineType::RadialBlur] = instance->radialBlurPipeLine_->SetUpPso();

	// Randome
	instance->randomPipeLine_ = make_unique<RandomPipeLine>();
	instance->pipeLineMap_[PipeLineType::Random] = instance->randomPipeLine_->SetUpPso();

	// SepiaTone
	instance->sepiaTonePipeLine_ = make_unique<SepiaTonePipeLine>();
	instance->pipeLineMap_[PipeLineType::SepiaTone] = instance->sepiaTonePipeLine_->SetUpPso();

	// Vignetting
	instance->vignettingPipeLine_ = make_unique<VignettingPipeLine>();
	instance->pipeLineMap_[PipeLineType::Vignetting] = instance->vignettingPipeLine_->SetUpPso();
}


// PipeLineのチェックと設定
void PipeLineManager::PipeLineCheckAndSet(const PipeLineType type)
{
	// PipeLineTypeが違っていたらcommandを再設定
	if (nowPipeLineType_ != type) {

		// Commandの取得
		Commands commands = CommandManager::GetInstance()->GetCommands();

		// このクラスのインスタンスの取得
		PipeLineManager* instance = PipeLineManager::GetInstance();

		// nowPipeLineTypeの再設定
		nowPipeLineType_ = type;

		// 引数typeにあったPipeLineを積みなおす
		commands.List->SetGraphicsRootSignature(instance->pipeLineMap_[type].rootSignature);
		commands.List->SetPipelineState(instance->pipeLineMap_[type].graphicsPipelineState);
		// 形状を設定。基本PipeLineで設定したものと同じもの
		commands.List->IASetPrimitiveTopology(instance->pipeLineMap_[type].primitiveTopologyType);
	}
}

