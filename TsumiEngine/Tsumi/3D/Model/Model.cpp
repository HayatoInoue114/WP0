#include "Model.h"
#include "ModelManager/ModelManager.h"
#include "../../Animation/KeyFrameAnimation/KeyFrameAnimation.h"


/// <summary>
/// コンストラクタ
/// </summary>
Model::Model() {

	// ModelManagerのインスタンス取得
	modelManager_ = ModelManager::Getinstance();

	// KeyFrameAnimationのインスタンス取得
	keyFrameAnimation_ = KeyFrameAnimation::GetInstance();
}


/// <summary>
/// 初期化処理
/// </summary>
void Model::Initialize(IModelState* state, WorldTransform worldTransform) {

	// ワールド座標のデフォルト設定
	this->worldTransform_ = worldTransform;

	// テクスチャの初期設定
	this->useTexture_ = 1;

	// 色の設定
	this->color_ = Vector4::one;

	// 光の設定
	this->light_.color = Vector4::one;
	this->light_.direction = { 0.0f, -1.0f, 0.0f };
	//this->light_.SpecularFColor = { 1.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 1.0f;
	this->light_.enableLightting = false;

	// モデルの描画タイプ
	this->modelDrawType_ = Phong;

	// ステートパターンの初期化処理
	this->state_ = state;
	this->state_->Initialize(this);
}


/// <summary>
/// Objファイルの読み込み & Obj初期化処理
/// </summary>
void Model::CreateFromObj(const std::string& routeFilePath, const std::string& fileName, WorldTransform worldTransform) {

	// ワールド座標のデフォルト設定
	this->worldTransform_ = worldTransform;

	// テクスチャの初期設定
	this->useTexture_ = 1;

	// 色の設定
	this->color_ = Vector4::one;

	// 光の設定
	this->light_.color = Vector4::one;
	this->light_.direction = { 0.0f, -1.0f, 0.0f };
	//this->light_.SpecularFColor = { 1.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 1.0f;
	this->light_.enableLightting = false;

	// Objファイルパス
	this->fileName_ = fileName;
	this->routeFilePath_ = routeFilePath;

	// Objの読み込み
	this->objData_ = modelManager_->LoadObjFile(routeFilePath, fileName);

	// モデルの描画タイプ
	this->modelDrawType_ = Phong;

	// ステートパターンの初期化処理
	this->state_ = new ModelObjState();
	this->state_->Initialize(this);
}

void Model::CreateFromObjAssimpVer(const std::string& routeFilePath, const std::string& fileName, WorldTransform worldTransform) {

	// ワールド座標のデフォルト設定
	this->worldTransform_ = worldTransform;

	// テクスチャの初期設定
	this->useTexture_ = 1;

	// 色の設定
	this->color_ = Vector4::one;

	// 光の設定
	this->light_.color = Vector4::one;
	this->light_.direction = { 0.0f, -1.0f, 0.0f };
	//this->light_.SpecularFColor = { 1.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 1.0f;
	this->light_.enableLightting = false;

	// Objファイルパス
	this->fileName_ = fileName;
	this->routeFilePath_ = routeFilePath;

	// Objの読み込み
	this->objData_ = modelManager_->LoadObjFileAssimpVer(routeFilePath, fileName);

	// モデルの描画タイプ
	this->modelDrawType_ = Phong;

	// ステートパターンの初期化処理
	this->state_ = new ModelObjState();
	this->state_->Initialize(this);
}

void Model::CreateGLTFModel(const std::string& routeFilePath, const std::string& fileName, const std::string& textureName, WorldTransform worldTransform) {

	// ワールド座標のデフォルト設定
	this->worldTransform_ = worldTransform;

	// テクスチャの初期設定
	this->useTexture_ = 1;

	// 色の設定
	this->color_ = Vector4::one;

	// 光の設定
	this->light_.color = Vector4::one;
	this->light_.direction = { 0.0f, -1.0f, 0.0f };
	//this->light_.SpecularFColor = { 1.0f, 1.0f, 1.0f };
	this->light_.intensity = 1.0f;
	this->light_.sininess = 1.0f;
	this->light_.enableLightting = false;

	// Objファイルパス
	this->fileName_ = fileName;
	this->routeFilePath_ = routeFilePath;

	// Objの読み込み
	this->objData_ = modelManager_->LoadGLTF(routeFilePath, fileName, textureName);

	// モデルの描画タイプ
	this->modelDrawType_ = Phong;

	// ステートパターンの初期化処理
	this->state_ = new ModelGLTFState();
	this->state_->Initialize(this);
}



/// <summary>
/// モデルの読み込み
/// </summary>
unique_ptr<Model> Model::LoadObjFileAssimpVer(const std::string& routeFilePath, const std::string& fileName)
{
	/* 1. 中で必要となる変数の宣言 */

	auto result = make_unique<Model>(); // return するModel
	string name = fileName.substr(0, fileName.size() - 4);

	// 返すModelにファイルの名前を付ける
	result->name_ = name;



	/* 2. ファイルを開く */

	// asssimpでobjを読む
	Assimp::Importer importer;
	string file = ("Resources/Obj/" + routeFilePath + "/" + fileName);



	/* 3. 実際にファイルを読み、ModelDataを構築していく */

	//三角形の並び順を逆にする         UVをフリップする(texcoord.y = 1.0f - texcoord.y;の処理)
	const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない


	// meshを解析する
	// 今回作るメッシュ
	unique_ptr<Mesh> meshItem = make_unique<Mesh>();

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは小名木は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応

		// ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート

			// ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex{};
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z, };
				vertex.texCoord = { texcoord.x, texcoord.y };

				// aiProcess_MakeKeftHanded は z *= -1 で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				// 解析した値を差し込む
				meshItem->meshData.vertices.push_back(vertex);
			}
		}
	}
	// 解析し終えたmeshを設定する
	meshItem->Create();
	result->meshMap_[name] = move(meshItem);


	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {

		// 今回作るマテリアル
		unique_ptr<MaterialModel> materialItem = make_unique<MaterialModel>();
		materialItem->Create();

		// シーン内のマテリアル
		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

			// テクスチャの読み込み
			materialItem->textureHandle = TextureManager::LoadTexture("Obj/" + routeFilePath, textureFilePath.C_Str());
		}

		// マテリアルの名前の設定
		// 複数マテリアルは今は非対応。ファイルの名前をそのままマテリアルの名前へ
		materialItem->name = material->GetName().C_Str();
		result->materialMap_[name] = move(materialItem);
	}


	return result;
}
unique_ptr<Model> Model::LoadGLTF(const std::string& routeFilePath, const std::string& fileName, const std::string& textureName)
{
	/* 1. 中で必要となる変数の宣言 */

	auto result = make_unique<Model>(); // return するModel
	unique_ptr<Mesh> meshItem = make_unique<Mesh>();
	string name = fileName.substr(0, fileName.size() - 4);

	// 返すModelにファイルの名前を付ける
	result->name_ = name;

	textureName;

	/* 2. ファイルを開く */

	// asssimpでobjを読む
	Assimp::Importer importer;
	string file = ("Resources/Obj/" + routeFilePath + "/" + fileName);



	/* 3. 実際にファイルを読み、ModelDataを構築していく */

	//三角形の並び順を逆にする         UVをフリップする(texcoord.y = 1.0f - texcoord.y;の処理)
	const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない


	// meshを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals()); // 法線がないMeshは小名木は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応

		// ここからMeshの中身(Face)の解析を行っていく
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート

			// ここからFaceの中身(Vertex)の解析を行っていく
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];

				VertexData vertex{};
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z, };
				vertex.texCoord = { texcoord.x, texcoord.y };

				// aiProcess_MakeKeftHanded は z *= -1 で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;

				// 解析した値を差し込む
				meshItem->meshData.vertices.push_back(vertex);
			}
		}
	}
	// 解析し終えたmeshを設定する
	result->meshMap_[name] = move(meshItem);


	// materialを解析する
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {

		// 今回作るマテリアル
		unique_ptr<MaterialModel> materialItem = make_unique<MaterialModel>();

		// マテリアルを作る
		materialItem->Create();

		// シーン内のマテリアル
		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {

			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

			// テクスチャの読み込み
			materialItem->textureHandle = TextureManager::LoadTexture("Obj/" + routeFilePath, textureFilePath.C_Str());
		}

		// マテリアルの名前の設定
		materialItem->name = material->GetName().C_Str();
		result->materialMap_[name] = move(materialItem);
	}


	return result;
}


/// <summary>
/// 描画処理
/// </summary>
void Model::Draw(WorldTransform worldTransform, Camera* camera) 
{
	this->state_->Draw(this, worldTransform, camera);
}
void Model::AnimDraw(WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera)
{
	this->state_->AnimDraw(this, worldTransform, skinCluster, camera);
}
void Model::DrawN(Transform transform, Camera* camera)
{
	// 諸々の計算
	transform.UpdateMatrix();
	transform.World = transform.matWorld;
	transform.WVP = camera->projectionMatrix * camera->viewMatrix * transform.matWorld;
	transform.WorldInverseTranspose = Transpose(Inverse(transform.matWorld));

	// ここで書き込み
	meshMap_.at(name_)->TransferMesh();
	materialMap_.at(name_)->TransferMaterial();
	transform.TransferMatrix();

	// コマンドコール
	CommandCall(transform, camera);
}


/// <summary>
/// Animationの再生
/// </summary>
void Model::PlayAnimation(Animation animation, float time)
{
	// GLTFで読み込んだモデルじゃないとダメ
	assert(this->state_->GetStateType() == ModelStateType::gLTF && "その読み込み形式はだめだよ");

	// timeにあったAnimationを解析して、LocalMatrixに入れる
	this->objData_.rootNode.localMatrix = keyFrameAnimation_->PlayAnimation(this->objData_.rootNode.name, animation, time);
}


/// <summary>
/// Nodeの階層構造からSkeletonを作る
/// </summary>
Skeleton Model::CreateSkeleton()
{
	// GLTFで読み込んだモデルじゃないとダメ
	assert(this->state_->GetStateType() == ModelStateType::gLTF && "その読み込み形式はだめだよ");

	return modelManager_->CreateSkeleton(this->objData_.rootNode);
}


/// <summary>
/// Skeletonの更新処理
/// </summary>
void Model::UpdateSkeleton(Skeleton& skeleton)
{
	// すべてのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton.joints) {

		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);

		// 親がいれば親の行列を掛ける
		if (joint.parent) {

			joint.skeletonSpaceMatrix = joint.localMatrix * skeleton.joints[*joint.parent].skeletonSpaceMatrix;
		}
		else { // 親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する

			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}


/// <summary>
/// Animationを適用する
/// </summary>
void Model::ApplyAnimation(Skeleton& skeleton, const Animation& animation, float animationTime)
{
	keyFrameAnimation_->ApplyAnimation(skeleton, animation, animationTime);
}


/// <summary>
/// SKinClusterの生成
/// </summary>
SkinCluster Model::CreateSkinCluster(const Skeleton& skeleton)
{
	// 作成するSkinCluster
	SkinCluster result{};

	// palette用のResourceを確保
	result.paletteResource = CreateResource::CreateBufferResource(sizeof(WellForGPU) * skeleton.joints.size());
	WellForGPU* mappedPalette = nullptr;
	result.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	result.mappedPallette = { mappedPalette, skeleton.joints.size() }; // spanを使ってアクセスするようにする
	result.srvHandle = SRVManager::CreateSkinClusterSRV(result.paletteResource, skeleton); // SRVHandleの設定

	// influence用のResourceを確保
	result.influenceResource = CreateResource::CreateBufferResource(sizeof(VertexInfluence) * this->objData_.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	result.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * this->objData_.vertices.size()); // 0埋め。weightを0にしておく
	result.mappedInfluence = { mappedInfluence, this->objData_.vertices.size() };

	// influence用のVBVを作成
	result.influenceBufferView.BufferLocation = result.influenceResource->GetGPUVirtualAddress();
	result.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * this->objData_.vertices.size());
	result.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	// InverseBindPoseMatrixの保存領域を作成。単位行列で埋める
	result.inverseBindPosematrices.resize(skeleton.joints.size());
	std::generate(result.inverseBindPosematrices.begin(), result.inverseBindPosematrices.end(), []() { return Matrix4x4::identity; });

	// ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める
	for (const auto& jointWeight : this->objData_.skinClusterData) { // ModelのSkinClusterの情報を解析
		auto it = skeleton.jointMap.find(jointWeight.first); // jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		if (it == skeleton.jointMap.end()) { // そんな名前のjointは存在しない。なので次に回す
			continue;
		}

		// (*it).secondにはjointのindexが入っているので、該当indexのinverseBindPoseMatrixを代入
		result.inverseBindPosematrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = result.mappedInfluence[vertexWeight.vertexIndex]; // 該当vertexIndexのinfluence情報を参照しておく
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) { // 空いているところに入れる
				if (currentInfluence.weights[index] == 0.0f) { // weight == 0 が開いている状態なので、その場所にweightとjointのindexを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return result;
}


/// <summary>
/// SkinCLusterの更新処理
/// </summary>
void Model::UpdateSkinCluster(SkinCluster& skinCluster, const Skeleton& skeleton)
{
	for (size_t jointIndex = 0; jointIndex < skeleton.joints.size(); ++jointIndex) {

		assert(jointIndex < skinCluster.inverseBindPosematrices.size());

		skinCluster.mappedPallette[jointIndex].skeletonSpaceMatrix =
			skinCluster.inverseBindPosematrices[jointIndex] * skeleton.joints[jointIndex].skeletonSpaceMatrix;

		skinCluster.mappedPallette[jointIndex].skeletonSpaceInverseTransposeMatrix =
			Transpose(Inverse(skinCluster.mappedPallette[jointIndex].skeletonSpaceMatrix));
	}
}


/// <summary>
/// mtlファイルを読み込む関数
/// </summary>
MaterialModel* Model::LoadMaterialTemplateFile(const std::string& filePath, const std::string& fileName)
{
	/* 1. 中で必要となる変数の宣言 */

	MaterialModel* result = nullptr; // 構築するMaterialData
	string line{};  // ファイルから読んだ１行を格納するもの


	/* 2. ファイルを開く */

	// ファイルを開く
	std::ifstream file("Resources/Obj/" + filePath + "/" + fileName);

	//とりあえず開けなかったら止める
	assert(file.is_open());


	/* 3. 実際にファイルを読み、MaterualDataを構築していく */

	while (std::getline(file, line)) {

		std::string identifier{};
		std::istringstream s(line);
		s >> identifier;

		// 新規マテリルの作成
		if (identifier == "newmtl") {

			// 新規マテリアルならMapに追加
			string mtlName;
			s >> mtlName;
			materialMap_[mtlName] = make_unique<MaterialModel>(mtlName);
			result = materialMap_[mtlName].get();

			// 仮テクスチャハンドル
			result->textureHandle = TextureManager::LoadTexture("Texture/", "uvChecker.png");
		}

		// identifierに応じた処理
		// "map_Kd" = textureのファイル名が記載されている

		else if (identifier == "map_Kd") {

			std::string textureFileName{};
			s >> textureFileName;
			textureFileName = textureFileName.substr(0, fileName.size() - 4);

			// 画像を読み込む
			if (result != nullptr) {
				textureFileName = textureFileName.substr(0, fileName.size() - 4);
				result->textureHandle = TextureManager::LoadTexture("Obj/" + filePath, textureFileName);
			}
			else {
				//とりあえず止める
				assert(result == nullptr);
			}
		}
	}

	return result;
}



/// <summary>
/// コマンドコール
/// </summary>
void Model::CommandCall(Transform transform, Camera* camera)
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Object3D);

	// コマンドを詰む
	commands.List->IASetVertexBuffers(0, 1, &meshMap_.at(name_)->vertexBufferView); // VBV
	commands.List->SetGraphicsRootConstantBufferView(0, materialMap_.at(name_)->constBuffer->GetGPUVirtualAddress()); // Material
	commands.List->SetGraphicsRootConstantBufferView(1, transform.constBuffer->GetGPUVirtualAddress()); // TransformationMatrix
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress()); // TransformationViewMatrix
	if (!materialMap_.at(name_)->textureHandle == 0) {
		//DescriptorManager::SetGraphicsRootDescriptorTable(3, materialMap_.at(name_)->textureHandle); // Texture
		SRVManager::SetGraphicsRootDescriptorTable(3, materialMap_.at(name_)->textureHandle);
	}
	commands.List->DrawInstanced(UINT(meshMap_.at(name_)->meshData.vertices.size()), 1, 0, 0); // Draw!!
}
