#include "ParticleRenderer.h"



// 　初期化処理
void ParticleRenderer::Initialize(uint32_t maxInstance)
{
	// 使うテクスチャは設定していなかったらuvChecker
	useTexture_ = 0;

	// 最大描画数
	instanceNum_ = maxInstance;

	// リソースの作成
	resource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * 4);
	resource_.Material = CreateResource::CreateBufferResource(sizeof(MaterialParticle));
	resource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * 4, resource_.Vertex.Get(), 4);

	resource_.Index = CreateResource::CreateBufferResource(sizeof(uint32_t) * 6);
	resource_.IndexBufferView = CreateResource::CreateIndexBufferview(sizeof(uint32_t) * 6, resource_.Index.Get());

	resource_.instancing = CreateResource::CreateBufferResource(sizeof(ParticleTransformationMatrix) * instanceNum_);
	//dsvIndex_ = DescriptorManager::CreateInstancingSRV(instanceNum_, resource_.instancing, sizeof(ParticleTransformationMatrix));
	dsvIndex_ = SRVManager::CreateInstancingSRV(instanceNum_, resource_.instancing, sizeof(ParticleTransformationMatrix));
}


// 描画処理
void ParticleRenderer::Draw(const list<unique_ptr<IParticle>>& p, Camera* camera)
{
	VertexData* vertexData = nullptr;
	MaterialParticle* materialData = nullptr;
	uint32_t* indexData = nullptr;
	ParticleTransformationMatrix* instancingData = nullptr;

	// 書き込みができるようにする
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	resource_.instancing->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));


	// 左下
	vertexData[0].position = { pos_.x - size_, pos_.y - size_, pos_.z, 1.0f };
	vertexData[0].texCoord = { 0.0f, 1.0f };
	vertexData[0].normal = { 0.0f, 1.0f, 0.0f };

	// 左上
	vertexData[1].position = { pos_.x - size_, pos_.y + size_, pos_.z, 1.0f };
	vertexData[1].texCoord = { 0.0f, 0.0f };
	vertexData[1].normal = { 0.0f, 1.0f, 0.0f };

	// 右下
	vertexData[2].position = { pos_.x + size_, pos_.y - size_, pos_.z, 1.0f };
	vertexData[2].texCoord = { 1.0f, 1.0f };
	vertexData[2].normal = { 0.0f, 1.0f, 0.0f };

	// 右上
	vertexData[3].position = { pos_.x + size_, pos_.y + size_, pos_.z, 1.0f };
	vertexData[3].texCoord = { 1.0f, 0.0f };
	vertexData[3].normal = { 0.0f, 1.0f, 0.0f };


	// 頂点インデックス
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 1; indexData[4] = 3; indexData[5] = 2;


	// マテリアルの設定
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	materialData->uvTransform =
		MakeAffineMatrix(Vector3::one, Vector3::zero, Vector3::zero);

	// ビルボードの計算
	Matrix4x4 billMat = CalcBillBord(camera);


	instanceNum_ = 0;
	for (std::list<unique_ptr<IParticle>>::const_iterator itr = p.begin(); itr != p.end(); ++itr) {

		// 最大描画数を超えないようにする
		if (instanceNum_ <= kMaxInstanceNum_) {

			Matrix4x4 scaleMat = MakeScaleMatrix((*itr)->GetTransform().srt.scale);
			Matrix4x4 translateMat = MakeTranslateMatrix((*itr)->GetTransform().srt.translate);
			Matrix4x4 worldPos = scaleMat * (billMat * translateMat);
			Matrix4x4 worldView = camera->viewMatrix * camera->projectionMatrix;
			Matrix4x4 matWorld = worldPos * worldView;

			(*itr)->GetUvTransform().matWorld = MakeAffineMatrix(
				(*itr)->GetUvTransform().srt.scale, (*itr)->GetUvTransform().srt.rotate, (*itr)->GetUvTransform().srt.translate);

			instancingData[instanceNum_].World = matWorld;
			instancingData[instanceNum_].Color = (*itr)->GetColor();
			instancingData[instanceNum_].uvTansform = (*itr)->GetUvTransform().matWorld;
			instanceNum_++;
		}
		++itr;
	}

	// コマンドコール
	CommandCall();
}


// コマンドコール
void ParticleRenderer::CommandCall()
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Particle);

	///// いざ描画！！！！！
	// VBVを設定
	commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	//DescriptorManager::SetGraphicsRootDescriptorTable(1, dsvIndex_);
	SRVManager::SetGraphicsRootDescriptorTable(1, dsvIndex_);


	// Texture
	if (!useTexture_ == 0) {
		//DescriptorManager::SetGraphicsRootDescriptorTable(2, useTexture_);
		SRVManager::SetGraphicsRootDescriptorTable(2, useTexture_);
	}

	// 描画！(DrawCall / ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
	commands.List->DrawIndexedInstanced(6, instanceNum_, 0, 0, 0);
}


// ビルボードの処理
Matrix4x4 ParticleRenderer::CalcBillBord(Camera* camera)
{
	Matrix4x4 backToFrontMat = Matrix4x4::identity;
	Matrix4x4 billBoardMat = backToFrontMat * camera->rotateMat;
	billBoardMat.m[3][0] = 0.0f;
	billBoardMat.m[3][1] = 0.0f;
	billBoardMat.m[3][2] = 0.0f;
	return billBoardMat;
}

