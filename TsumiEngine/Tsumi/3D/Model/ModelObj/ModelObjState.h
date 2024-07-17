#pragma once

#include "../../../Base/DXCommon/DirectXCommon.h"
#include "../../../CommandManager/CommandManager.h"
#include "../../../TextureManager/TextureManager.h"
#include "../../../PipeLineManager/PipeLineManager.h"
#include "../../../CreateResource/CreateResource.h"
#include "../../../DescriptorManager/DescriptorManager.h"
#include "../IModelState.h"
#include "../../../Transform/WorldTransform/WorldTransform.h"
#include "../../../Project/Math/MyMath.h"
#include "../../../Project/Math/Struct.h"
#include "../../../Project/GameObject/Camera/Camera.h"

#include <fstream>
#include <sstream>

class Model;

/* ModelObjクラス */
class ModelObjState : public IModelState {

public: // メンバ関数

	ModelObjState() { statetype_ = Obj; };
	~ModelObjState() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* pModel) override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) override;
	void AnimDraw(Model* pModel, WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera) override;


private: // メンバ関数

	/// <summary>
	/// コマンドコール処理
	/// </summary>
	void CommandCall(Model* pModel, WorldTransform worldTransform, Camera* camera);

private: // メンバ関数

	// リソース
	ResourcePeroperty resource_{};

	// ModelData
	ModelData modelData_{};
};