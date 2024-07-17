#pragma once

#include "../../CommandManager/CommandManager.h"
#include "../../View/SRVManager/SRVManager.h"
#include "../../Transform/WorldTransform/WorldTransform.h"
#include "../../Transform/Transform.h"
#include "../../PipeLineManager/PipeLineManager.h"
#include "IModelState.h"
#include "ModelGLTF/ModelGLTFState.h"
#include "ModelObj/ModelObjState.h"
#include "ModelObj/ObjDataResource/ObjDataResource.h"
#include "../../Animation/AnimationManager/AnimationManager.h"
#include "ModelStructure/Mesh/Mesh.h"
#include "ModelStructure/Material/Material.h"


//#include "Node.h"
//#include "Skeleton.h"


class ModelManager;
class KeyFrameAnimation;


//// 繝｢繝・Ν繝・・繧ｿ
//struct ModelData {
//	std::map<string, JointWeightData> skinClusterData;
//	std::vector<VertexData> vertices;
//	std::vector<uint32_t> indices;
//	MaterialData material;
//	uint32_t textureHD;
//	Node rootNode;
//};
//// Obj繝・・繧ｿ
//struct ObjData {
//	uint32_t index;
//	vector<VertexData> vertices;
//	MaterialData material;
//	Node node;
//};


/* Model繧ｯ繝ｩ繧ｹ */
class Model {

public: // 繝｡繝ｳ繝宣未謨ｰ

	/// <summary>
	/// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	/// </summary>
	Model();

	/// <summary>
	/// 繝・せ繝医Λ繧ｯ繧ｿ
	/// </summary>
	~Model() {};

	/// <summary>
	/// 蛻晄悄蛹門・逅・
	/// </summary>
	void Initialize(IModelState* state, WorldTransform worldTransform = WorldTransform());

	/// <summary>
	/// Obj繝輔ぃ繧､繝ｫ縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ & Obj蛻晄悄蛹門・逅・
	/// </summary>
	void CreateFromObj(const std::string& routeFilePath, const std::string& fileName, WorldTransform worldTransform = WorldTransform());
	void CreateFromObjAssimpVer(const std::string& routeFilePath, const std::string& fileName, WorldTransform worldTransform = WorldTransform());
	void CreateGLTFModel(const std::string& routeFilePath, const std::string& fileName, const std::string& textureName, WorldTransform worldTransform = WorldTransform());


	/// <summary>
	/// 繝｢繝・Ν縺ｮ隱ｭ縺ｿ霎ｼ縺ｿ
	/// </summary>
	static unique_ptr<Model> LoadObjFileAssimpVer(const std::string& routeFilePath, const std::string& fileName);
	static unique_ptr<Model> LoadGLTF(const std::string& routeFilePath, const std::string& fileName, const std::string& textureName);

	/// <summary>
	/// 謠冗判蜃ｦ逅・
	/// </summary>
	void Draw(WorldTransform worldTransform, Camera* camera);
	void AnimDraw(WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera);
	void DrawN(Transform transform, Camera* camera);

	/// <summary>
	/// Animation縺ｮ蜀咲函
	/// </summary>
	void PlayAnimation(Animation animation, float time);

	/// <summary>
	/// Node縺ｮ髫主ｱ､讒矩縺九ｉSkeleton繧剃ｽ懊ｋ
	/// </summary>
	Skeleton CreateSkeleton();

	/// <summary>
	/// Skeleton縺ｮ譖ｴ譁ｰ蜃ｦ逅・
	/// </summary>
	void UpdateSkeleton(Skeleton& skeleton);

	/// <summary>
	/// Animation繧帝←逕ｨ縺吶ｋ
	/// </summary>
	void ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime);


	/// <summary>
	/// SKinCluster縺ｮ逕滓・
	/// </summary>
	SkinCluster CreateSkinCluster(const Skeleton& skeleton);


	/// <summary>
	/// SkinCLuster縺ｮ譖ｴ譁ｰ蜃ｦ逅・
	/// </summary>
	void UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton);

#pragma region Get

	// WorldTransform
	WorldTransform GetWorldTransform() { return this->worldTransform_; }

	// UseTexture
	uint32_t GetUseTexture() const { return this->useTexture_; }

	// NormalMapTexture
	uint32_t GetNormalMapTex() const { return this->normalMapTex_; }

	// Color
	Vector4 GetColor() const { return this->color_; }

	// DirectionalLight
	DirectionalLight GetDirectionalLight() const { return this->light_; }

	// SphereRadius
	float GetRadius() const { return this->radius_; }

	// fileName_
	const std::string GetObjFileName() { return this->fileName_; }

	// ObjHandle
	uint32_t GetObjHandle() const { return this->objHandle_; }

	// ObjData
	ModelData GetObjData() { return this->objData_; }

	// 繝ｩ繧､繝・ぅ繝ｳ繧ｰ縺ｮ繧ｿ繧､繝・
	ModelLightingType GetModelDrawType() const { return this->modelDrawType_; }

	// Node
	Node GetNode() const { return this->objData_.rootNode; }

#pragma endregion 


#pragma region Set

	// TextureHandle
	void SetTexHandle(uint32_t texHD) { this->useTexture_ = texHD; }

	// NormalMapTexture
	void SetNormalMapTex(uint32_t texHD) { this->normalMapTex_ = texHD; }

	// Color
	void SetColor(Vector4 color) { this->color_ = color; }

	// Light
	void SetDirectionalLight(DirectionalLight light) { this->light_ = light; }

	// DrawType
	void SetModelDrawType(ModelLightingType type) { this->modelDrawType_ = type; }

	// Node
	void SetNode(Node node) { this->objData_.rootNode = node; }

	// Node.localMatrix
	void SetNodeMatrix(Matrix4x4 setLocalMat) { this->objData_.rootNode.localMatrix = setLocalMat; }

#pragma endregion


private:

	/// <summary>
	/// mtl繝輔ぃ繧､繝ｫ繧定ｪｭ縺ｿ霎ｼ繧髢｢謨ｰ
	/// </summary>
	MaterialModel* LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName);

	/// <summary>
	/// 繧ｳ繝槭Φ繝峨さ繝ｼ繝ｫ
	/// </summary>
	void CommandCall(Transform transform, Camera* camera);

private: // 繝｡繝ｳ繝仙､画焚

	// 繝｢繝・Ν繝槭ロ繝ｼ繧ｸ繝｣繝ｼ
	ModelManager* modelManager_ = nullptr;

	// 繧ｭ繝ｼ繝輔Ξ繝ｼ繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ
	KeyFrameAnimation* keyFrameAnimation_ = nullptr;

	// 繧ｹ繝・・繝医ヱ繧ｿ繝ｼ繝ｳ
	IModelState* state_ = nullptr;

	// 繝ｯ繝ｼ繝ｫ繝牙ｺｧ讓・
	WorldTransform worldTransform_{};

	// 繝・け繧ｹ繝√Ε
	uint32_t useTexture_{};
	uint32_t normalMapTex_{};

	// 濶ｲ繝・・繧ｿ
	Vector4 color_{};

	// 蜈峨ョ繝ｼ繧ｿ
	DirectionalLight light_{};

	// 繧ｹ繝輔ぅ繧｢縺ｮ蜊雁ｾ・
	float radius_ = 1.0f;

	// Obj縺ｮ繝輔ぃ繧､繝ｫ繝代せ
	std::string fileName_{};
	std::string routeFilePath_{};

	// Obj縺ｮ繝上Φ繝峨Ν
	uint32_t objHandle_{};

	ModelData modelData_{};

	ModelData objData_{};

	ModelLightingType modelDrawType_ = Non;


	// 竊凪・竊凪・竊凪・竊凪・ 縺薙▲縺九ｉ譁ｰ縺励＞繝｢繝・Ν謠冗判縺ｫ蠢・ｦ√↑蛟､

	// Model縺ｮ蜷榊燕
	string name_;

	// Mesh繝・・繧ｿ
	unordered_map<string, unique_ptr<Mesh>> meshMap_;

	// Material繝・・繧ｿ
	unordered_map<string, unique_ptr<MaterialModel>> materialMap_;

};