#pragma once

#include "../../../../Project/Math/MyMath.h"
#include "../../../../Project/Math/Struct.h"
#include "../../../TextureManager/TextureManager.h"
#include "../ModelObj/ObjDataResource/ObjDataResource.h"
#include "../Model.h"
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;


/* ModelManagerクラス */
class ModelManager {

private: // シングルトンデザインパターン

	// コンストラクタ、デストラクタ
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static ModelManager* Getinstance() {
		static ModelManager instance;
		return &instance;
	}

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// モデルデータを追加する
	/// </summary>
	void const AddModel(const string name, unique_ptr<Model> model);

	/// <summary>
	/// モデルデータの取得
	/// </summary>
	Model* const GetModel(const string name) const;

	/// <summary>
	/// 指定のモデルデータの破棄
	/// </summary>
	void ModelRemove (string name);

	/// <summary>
	/// 全てのモデルデータの破棄
	/// </summary>
	void AllRemove();


	/// <summary>
	/// Objファイルを読み込む
	/// </summary>
	ModelData LoadObjFile(const std::string& routeFilePath, const std::string& fileName);
	ModelData LoadObjFileAssimpVer(const std::string& routeFilePath, const std::string& fileName);
	ModelData LoadGLTF(const std::string& routeFilePath, const std::string& fileName, const std::string& textureName);

	/// <summary>
	/// Nodeの階層構造からSkeletonを作る
	/// </summary>
	Skeleton CreateSkeleton(const Node& rootNode);


#pragma region Get

#pragma endregion 


private: // メンバ関数

	/// <summary>
	/// 一回読み込んだものは読み込まない
	/// </summary>
	bool CheckObjData(std::string filePath);

	/// <summary>
	/// mtlファイルを読み込む関数
	/// </summary>
	MaterialData LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName, const std::string& routeFilePath);

	/// <summary>
	/// Nodeの情報を読む
	/// </summary>
	Node ReadNode(aiNode* node);

	/// <summary>
	/// NodeからJointを作る
	/// </summary>
	int32_t CreateJoint(const Node& node, const optional<int32_t>& parent, vector<Joint>& joints);


private: // メンバ変数

	// ObjDataResource
	map<std::string, unique_ptr<ObjDataResource>> objModelDatas_;

	uint32_t objHandle_ = 0;

	ModelData modelData_{};


	// モデルのマップ
	unordered_map<string, unique_ptr<Model>> modelsMap_;
};

