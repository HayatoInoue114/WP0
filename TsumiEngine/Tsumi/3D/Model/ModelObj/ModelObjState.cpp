#include "ModelObjState.h"
#include "../Model.h"


/// <summary>
/// 初期化処理
/// </summary>
void ModelObjState::Initialize(Model* pModel) {

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
void ModelObjState::Draw(Model* pModel, WorldTransform worldTransform, Camera* camera) {

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


	// コマンドコール
	CommandCall(pModel, worldTransform, camera);
}

void ModelObjState::AnimDraw(Model* pModel, WorldTransform worldTransform, SkinCluster skinCluster, Camera* camera)
{
	pModel;
	worldTransform;
	skinCluster;
	camera;
}


/// <summary>
/// コマンドコール処理
/// </summary>
void ModelObjState::CommandCall(Model* pModel, WorldTransform worldTransform, Camera* camera) {

	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Phong);

	///// いざ描画！！！！！
	// 頂点の設定
	commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, worldTransform.constBuffer->GetGPUVirtualAddress());

	// View用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	if (pModel->GetObjData().textureHD != 0) {
		SRVManager::SetGraphicsRootDescriptorTable(3, pModel->GetObjData().textureHD);
	}

	// 光用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(4, resource_.Lighting->GetGPUVirtualAddress());

	// ノーマルマップ用のテクスチャの設定
	if (pModel->GetModelDrawType() == PhongNormalMap) {
		SRVManager::SetGraphicsRootDescriptorTable(5, pModel->GetObjData().textureHD);
	}

	// 描画！(DrawCall / ドローコール)。
	commands.List->DrawIndexedInstanced(UINT(modelData_.indices.size()), 1, 0, 0, 0);
}
