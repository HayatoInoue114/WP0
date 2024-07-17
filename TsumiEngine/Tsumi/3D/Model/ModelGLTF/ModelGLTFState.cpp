#include "ModelGLTFState.h"
#include "../Model.h"


/// <summary>
/// 初期化処理
/// </summary>
void ModelGLTFState::Initialize(Model* pModel) {

	modelData_ = pModel->GetObjData();

	// リソースの作成
	resource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	resource_.Material = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.Lighting = CreateResource::CreateBufferResource(sizeof(DirectionalLight));
	resource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * modelData_.vertices.size(), resource_.Vertex.Get(), int(modelData_.vertices.size()));


	resource_.Index = CreateResource::CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());
	resource_.IndexBufferView = CreateResource::CreateIndexBufferview(sizeof(uint32_t) * modelData_.indices.size(), resource_.Index.Get());
}


/// <summary>
/// 描画処理
/// </summary>
void ModelGLTFState::Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) {

	VertexData* vertexData = nullptr;
	Material* material = nullptr;
	DirectionalLight* lightData = nullptr;
	uint32_t* indexData = nullptr;


	// 書き込むためにアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&material));
	resource_.Lighting->Map(0, nullptr, reinterpret_cast<void**>(&lightData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));


	// 頂点データをリソースにコピー
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	std::memcpy(indexData, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());


	// マテリアルの情報を書き込む
	material->color = pModel->GetColor();


	// ライティングの設定
	(*lightData) = pModel->GetDirectionalLight();


	// RootのMatrixを適用する
	//worldTransform.matWorld = pModel->GetNode().localMatrix * worldTransform.matWorld;
	//worldTransform.TransferMatrix();


	// コマンドコール
	CommandCall(pModel, worldTransform, camera);
}

void ModelGLTFState::AnimDraw(Model* pModel, WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera)
{
	VertexData* vertexData = nullptr;
	Material* material = nullptr;
	DirectionalLight* lightData = nullptr;
	uint32_t* indexData = nullptr;


	// 書き込むためにアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&material));
	resource_.Lighting->Map(0, nullptr, reinterpret_cast<void**>(&lightData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));


	// 頂点データをリソースにコピー
	std::memcpy(vertexData, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size());
	std::memcpy(indexData, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());


	// マテリアルの情報を書き込む
	material->color = pModel->GetColor();


	// ライティングの設定
	(*lightData) = pModel->GetDirectionalLight();


	// RootのMatrixを適用する
	/*worldTransform.matWorld = pModel->GetNode().localMatrix * worldTransform.matWorld;
	worldTransform.TransferMatrix();*/


	// コマンドコール
	AnimCommandCall(pModel, worldTransform, skinCluster, camera);
}


/// <summary>
/// コマンドコール処理
/// </summary>
void ModelGLTFState::CommandCall(Model* pModel, WorldTransform worldTransform, Camera* camera) {

	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Phong);

	///// いざ描画！！！！！
	// VBVを設定
	commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer->GetGPUVirtualAddress());

	// View用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	if (!pModel->GetObjData().textureHD == 0) {
		//DescriptorManager::SetGraphicsRootDescriptorTable(3, pModel->GetObjData().textureHD);
		SRVManager::SetGraphicsRootDescriptorTable(3, pModel->GetObjData().textureHD);
	}

	// 光用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(4, resource_.Lighting->GetGPUVirtualAddress());

	// ノーマルマップ用のテクスチャの設定
	if (pModel->GetModelDrawType() == PhongNormalMap) {
		//DescriptorManager::SetGraphicsRootDescriptorTable(5, pModel->GetNormalMapTex());
		SRVManager::SetGraphicsRootDescriptorTable(5, pModel->GetObjData().textureHD);
	}

	// 描画！(DrawCall / ドローコール)。
	commands.List->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}

void ModelGLTFState::AnimCommandCall(Model* pModel, WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera)
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// SRVManagerの取得
	SRVManager* srvManager = SRVManager::GetInstance();

	// PipeLineCheck
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::SkinningObject3D);

	///// いざ描画！！！！！
	// VBVを設定
	//commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);
	D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
		resource_.VertexBufferView, // VertexDataのVBV
		skinCluster.influenceBufferView // InfluenceのVBV
	};
	// 配列を渡す(開始Slot番号、使用Slot数、VBV葉入れ悦へのポインタ)
	commands.List->IASetVertexBuffers(0, 2, vbvs);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// worldTransform
	commands.List->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer->GetGPUVirtualAddress());

	// camera
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	if (!pModel->GetObjData().textureHD == 0) {
		//DescriptorManager::SetGraphicsRootDescriptorTable(3, pModel->GetObjData().textureHD);
		srvManager->SetGraphicsRootDescriptorTable(3, pModel->GetObjData().textureHD);
	}

	// 光用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(4, resource_.Lighting->GetGPUVirtualAddress());

	// Skinning
	//commands.List->SetGraphicsRootDescriptorTable(5, skinCluster.paletteSrvHandle.second);
	srvManager->SetGraphicsRootDescriptorTable(5, skinCluster.srvHandle);

	// ノーマルマップ用のテクスチャの設定
	if (pModel->GetModelDrawType() == PhongNormalMap) {
		//DescriptorManager::SetGraphicsRootDescriptorTable(5, pModel->GetNormalMapTex());
		srvManager->SetGraphicsRootDescriptorTable(5, pModel->GetObjData().textureHD);
	}


	// 描画！(DrawCall / ドローコール)。
	commands.List->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}
