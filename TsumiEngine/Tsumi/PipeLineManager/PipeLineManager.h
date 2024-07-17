#pragma once


#include <d3dx12.h>
#include <dxcapi.h>

#include "../../../Project/Math/MyMath.h"
#include "../../../Project/Math/Struct.h"
#include "../Base/DXCommon/DirectXCommon.h"
#include "../ShaderManager/ShaderManager.h"
#include "../CommandManager/CommandManager.h"

#include "IPipeLineState/IPipeLineState.h"

#include "ModelPipeLine/LambertPipeLine/LambertPipeLine.h"
#include "ModelPipeLine/LightPipeLine/LightPipeLine.h"
#include "ModelPipeLine/LinePipeLine/LinePipeLine.h"
#include "ModelPipeLine/Object3DPipeLine/Object3DPipeLine.h"
#include "ModelPipeLine/ParticlePipeLine/ParticlePipeLine.h"
#include "ModelPipeLine/PhongNormalMapPipeLine/PhongNormalMapPipeLine.h"
#include "ModelPipeLine/PhongPipeLine/PhongPipeLine.h"
#include "ModelPipeLine/SkinningObject3dPipeLine/SkinningObject3dPipeLine.h"
#include "ModelPipeLine/SkyboxPipeLine/SkyboxPipeLine.h"
#include "ModelPipeLine/SpritePipeLine/SpritePipeLine.h"

#include "PostEffectPipeLine/AbsentPipeLine/AbsentPipeLine.h"
#include "PostEffectPipeLine/BoxFilterPipeLine/BoxFilterPipeLine.h"
#include "PostEffectPipeLine/DissolvePipeLine/DissolvePipeLine.h"
#include "PostEffectPipeLine/GaussianFilterPipeLine/GaussianFilterPipeLine.h"
#include "PostEffectPipeLine/GrayScalePipeLine/GrayScalePipeLine.h"
#include "PostEffectPipeLine/OutLinePipeLine/OutLinePipeLine.h"
#include "PostEffectPipeLine/RadialBlurPipeLine/RadialBlurPipeLine.h"
#include "PostEffectPipeLine/RandomPipeLine/RandomPipeLine.h"
#include "PostEffectPipeLine/SepiaTonePipeLine/SepiaTonePipeLine.h"
#include "PostEffectPipeLine/VignettingPipeLine/VignettingPipeLine.h"
#include "PostEffectPipeLine/GrainPipeLine/GrainPipeLine.h"


// PipeLineTypeのenum構造体
enum class PipeLineType : uint32_t {
	None,
	Lambert,
	Light,
	Line,
	Object3D,
	Particle,
	PhongNormalMap,
	Phong,
	SkinningObject3D,
	Skybox,
	Sprite,
	PostEffect,
	Absent,
	BoxFilter,
	Dissolve,
	GaussianFilter,
	Grain,
	GrayScale,
	OutLine,
	RadialBlur,
	Random,
	SepiaTone,
	Vignetting,
};


/* PipeLineManagerクラス */
class PipeLineManager {

public: // メンバ関数

	// インスタンスの取得
	static PipeLineManager* GetInstance() {
		static PipeLineManager instance;
		return &instance;
	}

	// 初期化処理
	static void Initialize();

	// PipeLineのチェックと設定
	static void PipeLineCheckAndSet(const PipeLineType type);

	// PipeLineTypeのリセット
	static void PipeLineTypeReset() {
		nowPipeLineType_ = PipeLineType::None;
	}

#pragma region Accessor アクセッサ

	// PipeLineの取得
	PsoProperty GetPipeLine(PipeLineType type) {
		return pipeLineMap_[type];
	}

#pragma endregion


private: // メンバ変数

	// PipeLineのタイプ
	static PipeLineType nowPipeLineType_;

	// PipeLineを保存しておくmapコンテナ
	std::map<PipeLineType, PsoProperty> pipeLineMap_;

	// Lambert
	std::unique_ptr<LambertPipeLine> lambertPipeLine_;

	// Light
	std::unique_ptr<LightPipeLine> lightPipeLine_;

	// Line
	std::unique_ptr<LinePipeLine> linePipeLine_;

	// Object3D
	std::unique_ptr<Object3DPipeLine> object3DPipeLine_;

	// Particle
	std::unique_ptr<ParticlePipeLine> particlePipeLine_;

	// PhongNormalMap
	std::unique_ptr<PhongNormalMapPipeLine> phongNormalMapPipeLine_;

	// Phong
	std::unique_ptr<PhongPipeLine> phongPipeLine_;

	// SkinningObject3d
	std::unique_ptr<SkinningObject3dPipeLine> skinningObject3DPipeLine_;

	// Skybox
	std::unique_ptr<SkyboxPipeLine> skyboxPipeLine_;

	// Sprite
	std::unique_ptr<SpritePipeLine> spritePipeLine_;


	// Absent
	std::unique_ptr<AbsentPipeLine> absentPipeLine_;

	// BoxFilter
	std::unique_ptr<BoxFilterPipeLine> boxFilterPipeLine_;

	// Dissolve
	std::unique_ptr<DissolvePipeLine> dissolvePipeLine_;

	// GaussianFilter
	std::unique_ptr<GaussianFilterPipeLine> gaussianFilterPipeLine_;

	// Grain
	std::unique_ptr<GrainPipeLine> grainPipeLine_;

	// GrayScale
	std::unique_ptr<GrayScalePipeLine> grayScalePipeLine_;

	// OutLine
	std::unique_ptr<OutLinePipeLine> outLinePipeLine_;

	// RadialBlur
	std::unique_ptr<RadialBlurPipeLine> radialBlurPipeLine_;

	// Random
	std::unique_ptr<RandomPipeLine> randomPipeLine_;

	// SepiaTone
	std::unique_ptr<SepiaTonePipeLine> sepiaTonePipeLine_;

	// Vignetting
	std::unique_ptr<VignettingPipeLine> vignettingPipeLine_;

};

