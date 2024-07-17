#include "ModelManager.h"



/// <summary>
/// 解放処理
/// </summary>
void ModelManager::Finalize() {

	ModelManager::Getinstance()->objModelDatas_.clear();
}


// モデルデータを追加する
void const ModelManager::AddModel(const string name, unique_ptr<Model> model)
{
	// 指定の名前で検索をかける
	auto modelData = GetModel(name);

	// ヒットしたら早期リターン
	if (modelData) { return; }

	// ヒットしなかったらマップに追加して return する
	modelsMap_[name] = move(model);
}


// モデルデータの取得
Model* const ModelManager::GetModel(const string name) const
{
	// 指定された名前で検索をかける
	auto model = modelsMap_.find(name);

	// ヒットしたらポインタを返す
	if (model != modelsMap_.end()) {
		return model->second.get();
	}
	return nullptr;
}


// 指定のモデルデータの破棄
void ModelManager::ModelRemove(string name)
{
	// 指定された名前で検索をかける
	const auto& model = modelsMap_.find(name);

	// なかったらリターン
	if (model == modelsMap_.end()) {
		return;
	}

	// ヒットしたら破棄する
	modelsMap_.erase(name);
}


// 全てのモデルデータの破棄
void ModelManager::AllRemove()
{
	modelsMap_.clear();
}


/// <summary>
/// Objファイルを読み込む
/// </summary>
ModelData ModelManager::LoadObjFile(const std::string& routeFilePath, const std::string& fileName) {

	if (CheckObjData(fileName)) {

		// 初めてだったら加算
		ModelManager::Getinstance()->objHandle_++;
		uint32_t modelHandle = ModelManager::Getinstance()->objHandle_;


		/* 1. 中で必要となる変数の宣言 */

		ModelData objData;            // 構築するModelData
		std::vector<Vector4> positions; // 位置
		std::vector<Vector3> normals;   // 法線
		std::vector<Vector2> texcoords; // テクスチャ座標
		std::string line;               // ファイルから読んだ１行をを格納するもの



		/* 2. ファイルを開く */

		// ファイルを開く
		std::ifstream file("Resources/Obj/" + routeFilePath + "/" + fileName + "/" + fileName + ".obj");
		// とりあえず開けなかったら止める
		assert(file.is_open());



		/* 3. 実際にファイルを読み、ModelDataを構築していく */

		while (std::getline(file, line)) {

			std::string identifier;
			std::istringstream s(line);

			// 先頭の識別子を読む
			s >> identifier;


			// identifierに応じた処理
			// "v"  = 頂点位置
			// "vt" = 頂点テクスチャ座標 
			// "vn" = 頂点法線
			// "f"  = 面

			if (identifier == "v") {

				Vector4 position{};
				s >> position.x >> position.y >> position.z;
				position.x *= -1.0f;
				position.w = 1.0f;
				positions.push_back(position);
			}
			else if (identifier == "vt") {

				Vector2 texcoord{};
				s >> texcoord.x >> texcoord.y;
				texcoord.y *= -1.0f;
				texcoords.push_back(texcoord);
			}
			else if (identifier == "vn") {

				Vector3 normal{};
				s >> normal.x >> normal.y >> normal.z;
				normal.x *= -1.0f;
				normals.push_back(normal);
			}
			else if (identifier == "f") {

				VertexData triangle[3]{};

				// 面は三角形限定。その他は未対応
				for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {

					std::string vertexDefinition;
					s >> vertexDefinition;

					// 頂点の要素へのIndexは「位置/UV/法線」で格納されているので、分解してIndexを取得する
					std::istringstream v(vertexDefinition);
					uint32_t elementIndex[3]{};

					for (int32_t element = 0; element < 3; ++element) {

						std::string index;
						std::getline(v, index, '/'); // "/"区切りでインデックスを読んでいく
						elementIndex[element] = std::stoi(index);
					}

					// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
					Vector4 position = positions[static_cast<std::vector<Vector4, std::allocator<Vector4>>::size_type>(elementIndex[0]) - 1];
					Vector2 texcoord = texcoords[static_cast<std::vector<Vector2, std::allocator<Vector2>>::size_type>(elementIndex[1]) - 1];
					Vector3 normal = normals[static_cast<std::vector<Vector3, std::allocator<Vector3>>::size_type>(elementIndex[2]) - 1];
					triangle[faceVertex] = { position, texcoord, normal };
				}
				// 頂点を逆順で登録することで、回利潤を逆にする
				objData.vertices.push_back(triangle[2]);
				objData.vertices.push_back(triangle[1]);
				objData.vertices.push_back(triangle[0]);
			}
			else if (identifier == "mtllib") {

				// materialTemplateLibraryファイルの名前を取得する
				std::string materialFileName;
				s >> materialFileName;

				// 基本的にobjファイルを同じ階層にmtlは存在させるので、ディレクトリファイル名を渡す
				objData.material = ModelManager::Getinstance()->LoadMaterialTemplateFile(fileName, materialFileName, routeFilePath);
			}
		}
		// テクスチャを指定されたものにする
		uint32_t texHandle = TextureManager::LoadTexture("Obj/" + routeFilePath, fileName);
		objData.textureHD = texHandle;
		ModelManager::Getinstance()->objModelDatas_[fileName] = make_unique<ObjDataResource>(objData, modelHandle);
	}

	return ModelManager::Getinstance()->objModelDatas_[fileName].get()->GetObjData();
}

ModelData ModelManager::LoadObjFileAssimpVer(const std::string& routeFilePath, const std::string& fileName)
{
	if (CheckObjData(fileName)) {

		// 初めてだったら加算
		ModelManager::Getinstance()->objHandle_++;
		uint32_t modelHandle = ModelManager::Getinstance()->objHandle_;
		ModelData objData{}; // モデルの格納用データ

		// asssimpでobjを読む
		Assimp::Importer importer;
		string file = ("Resources/Obj/" + routeFilePath + "/" + fileName + ".obj");

		                                                      //三角形の並び順を逆にする         UVをフリップする(texcoord.y = 1.0f - texcoord.y;の処理)
		const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
		assert(scene->HasMeshes()); // メッシュがないのは対応しない


		// meshを解析する
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
			aiMesh* mesh = scene->mMeshes[meshIndex];
			assert(mesh->HasNormals()); // 法線がないMeshは小名木は非対応
			assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応
			objData.vertices.resize(mesh->mNumVertices); // 最初に頂点数分のメモリを確保しておく

			for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				// 右手系 -> 左手系への変換
				objData.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
				objData.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
				objData.vertices[vertexIndex].texCoord = { texcoord.x, texcoord.y };
			}

			// Indexを解析する
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);

				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					objData.indices.push_back(vertexIndex);
				}
			}
		}

		// materialを解析する
		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
			aiMaterial* material = scene->mMaterials[materialIndex];

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePath;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				objData.material.textureFilePath = fileName + routeFilePath;
			}
		}

		// テクスチャを指定されたものにする
		uint32_t texHandle = TextureManager::LoadTexture("Obj/" + routeFilePath, fileName + ".png");
		objData.textureHD = texHandle;
		// 作ったモデルデータをデータ群に新しく作る
		ModelManager::Getinstance()->objModelDatas_[fileName] = make_unique<ObjDataResource>(objData, modelHandle);
	}

	// 同じファイルパスのモデルデータを検索して返す
	return ModelManager::Getinstance()->objModelDatas_[fileName].get()->GetObjData();
}

ModelData ModelManager::LoadGLTF(const std::string& routeFilePath, const std::string& fileName, const std::string& textureName)
{
	if (CheckObjData(fileName)) {

		// 初めてだったら加算
		ModelManager::Getinstance()->objHandle_++;
		uint32_t modelHandle = ModelManager::Getinstance()->objHandle_;
		ModelData objData{}; // モデルの格納用データ

		// asssimpでobjを読む
		Assimp::Importer importer;
		string file = ("Resources/gLTF/" + routeFilePath + "/" + fileName + ".gltf");

		//三角形の並び順を逆にする         UVをフリップする(texcoord.y = 1.0f - texcoord.y;の処理)
		const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
		assert(scene->HasMeshes()); // メッシュがないのは対応しない


		// meshを解析する
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
			aiMesh* mesh = scene->mMeshes[meshIndex];
			assert(mesh->HasNormals()); // 法線がないMeshは小名木は非対応
			assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応
			objData.vertices.resize(mesh->mNumVertices); // 最初に頂点数分のメモリを確保しておく

			for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				// 右手系 -> 左手系への変換
				objData.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
				objData.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
				objData.vertices[vertexIndex].texCoord = { texcoord.x, texcoord.y };
			}

			// Indexを解析する
			for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
				aiFace& face = mesh->mFaces[faceIndex];
				assert(face.mNumIndices == 3);

				for (uint32_t element = 0; element < face.mNumIndices; ++element) {
					uint32_t vertexIndex = face.mIndices[element];
					objData.indices.push_back(vertexIndex);
				}
			}

			// SkinCluster構築用のデータ種取得を追加
			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {

				// Jointごとの格納領域を作る
				aiBone* bone = mesh->mBones[boneIndex];
				string jointName = bone->mName.C_Str();
				JointWeightData& jointWeightData = objData.skinClusterData[jointName];

				// InverseBindPoseMatrixの抽出
				aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse(); // BindPoseMatrixに戻す
				aiVector3D scale, translate;
				aiQuaternion rotate;
				bindPoseMatrixAssimp.Decompose(scale, rotate, translate); // 成分を抽出
				// 左手系のBindPoseMatrixを作る
				Matrix4x4 bindPoseMatrix = MakeAffineMatrix(
					{ scale.x, scale.y, scale.z },
					{ rotate.w, rotate.x, -rotate.y, -rotate.z },
					{ -translate.x, translate.y, translate.z });
				// InverseBindPoseMatrixにする
				jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);
				Matrix4x4 hr = jointWeightData.inverseBindPoseMatrix * bindPoseMatrix;

				// Weight情報を取り出す
				for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
					jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
				}
			}
		}

		// materialを解析する
		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
			aiMaterial* material = scene->mMaterials[materialIndex];

			if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
				aiString textureFilePath;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
				objData.material.textureFilePath = fileName + routeFilePath;
				//objData.textureHD = TextureManager::LoadTexture("gLTF/" + routeFilePath, textureFilePath);
				//objData.material.textureFilePath = "gLTF/" + routeFilePath;
				//objData.textureHD = TextureManager::LoadTexture(objData.material.textureFilePath, textureFilePath.C_Str());
			}
		}

		// テクスチャを指定されたものにする
		uint32_t texHandle = TextureManager::LoadTexture("gLTF/" + routeFilePath, textureName);
		objData.textureHD = texHandle;
		TextureManager::LoadTexture("gLTF/" + routeFilePath, textureName);

		// Nodeを読み込む
		objData.rootNode = ModelManager::Getinstance()->ReadNode(scene->mRootNode);

		// 作ったモデルデータをデータ群に新しく作る
		ModelManager::Getinstance()->objModelDatas_[fileName] = make_unique<ObjDataResource>(objData, modelHandle);
	}

	// 同じファイルパスのモデルデータを検索して返す
	return ModelManager::Getinstance()->objModelDatas_[fileName].get()->GetObjData();
}


/// <summary>
/// Nodeの階層構造からSkeletonを作る
/// </summary>
Skeleton ModelManager::CreateSkeleton(const Node& rootNode)
{
	Skeleton skeleton{};
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}

	return skeleton;
}


/// <summary>
/// 一回読み込んだものは読み込まない
/// </summary>
bool ModelManager::CheckObjData(std::string filePath) {

	if (ModelManager::Getinstance()->objModelDatas_.find(filePath) == ModelManager::Getinstance()->objModelDatas_.end()) {

		return true;
	}

	return false;
}


/// <summary>
/// mtlファイルを読み込む関数
/// </summary>
MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName, const std::string& routeFilePath) {

	/* 1. 中で必要となる変数の宣言 */

	MaterialData materialData{}; // 構築するMaterialData
	std::string line{};			 // ファイルから読んだ１行を格納するもの


	/* 2. ファイルを開く */

	// ファイルを開く
	std::ifstream file("Resources/Obj/" + routeFilePath + "/" + filePath + "/" + fileName);

	//とりあえず開けなかったら止める
	assert(file.is_open());


	/* 3. 実際にファイルを読み、MaterualDataを構築していく */

	while (std::getline(file, line)) {

		std::string identifier{};
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		// "map_Kd" = textureのファイル名が記載されている

		if (identifier == "map_Kd") {

			std::string textureFileName{};
			s >> textureFileName;

			// 連結してファイルパスにする
			materialData.textureFilePath = "Resources/Obj/" + filePath + "/" + textureFileName;
		}
	}

	/* 4. MaterialData を返す */
	return materialData;
}


/// <summary>
/// Nodeの情報を読む
/// </summary>
Node ModelManager::ReadNode(aiNode* node)
{
	Node result;

	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // nodeのlocalMatrixを取得
	aiLocalMatrix.Transpose(); // 列ベクトル形式を行ベクトル形式に転置


	aiVector3D scale, translate;
	aiQuaternion rotate;

	// assimpの行列からSRTを抽出する関数を利用
	node->mTransformation.Decompose(scale, rotate, translate); 

	// scaleはそのまま
	result.transform.scale = { scale.x, scale.y,scale.z };

	// x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.rotate = { rotate.w, rotate.x, -rotate.y, -rotate.z };

	// x軸を反転
	result.transform.translate = { -translate.x, translate.y,translate.z };

	// 上記で読み込んだ情報を元にLocalMatrixを求める
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);

	result.name = node->mName.C_Str(); // Mode名を格納
	result.Children.resize(node->mNumChildren); // 子供の数だけ確保
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		// 再帰的によんで階層構造を作っていく
		result.Children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}
	return result;
}


/// <summary>
/// NodeからJointを作る
/// </summary>
int32_t ModelManager::CreateJoint(const Node& node, const optional<int32_t>& parent, vector<Joint>& joints)
{
	Joint joint{};
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = Matrix4x4::identity;
	joint.transform = node.transform;
	joint.index = int32_t(joints.size()); // 現在登録されている数をIndexに
	joint.parent = parent;

	// SkeletonのJoint列に追加
	joints.push_back(joint);

	for (const Node& child : node.Children) {

		// 子Jointを作成し、そのIndexを登録
		int32_t chileIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(chileIndex);
	}

	// 自身のIndexを返す
	return joint.index;
}

