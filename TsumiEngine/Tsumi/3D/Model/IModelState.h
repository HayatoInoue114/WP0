#pragma once

#include "../../Transform/WorldTransform/WorldTransform.h"
#include "../../Project/GameObject/Camera/Camera.h"


enum ModelStateType {
	Normal,
	Obj,
	gLTF,
};
enum ModelLightingType {
	Non,
	Lambert,
	Phong,
	PhongNormalMap,
};


// Modelの前方宣言
class Model;

/* IModelState モデルのステートパターン */
class IModelState {

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IModelState() {};


	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(Model* pModel) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) = 0;
	virtual void AnimDraw(Model* pModel, WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera) = 0;

	
#pragma region Get

	// StateType
	ModelStateType GetStateType() { return this->statetype_; }

#pragma endregion 


protected: // メンバ変数

	ModelStateType statetype_ = Normal;

};