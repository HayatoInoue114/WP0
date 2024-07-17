#include "Skybox.h"



// 初期化処理
void Skybox::Initialize()
{
	// テクスチャの読み込み
	this->texture_ = TextureManager::LoadTexture("Texture", "airport_4k.dds");

	// Scale
	initScale_ = 100.0f;

	// Transformの初期化処理
	transform_.Initialize();
	transform_.srt.scale = { initScale_, initScale_, initScale_ };

	// カラー
	color_ = Vector4::one;

	// リソースの作成
	resource_.Vertex = CreateResource::CreateBufferResource(sizeof(VertexData) * kNumVertices_);
	resource_.Material = CreateResource::CreateBufferResource(sizeof(Material));
	resource_.VertexBufferView = CreateResource::CreateVertexBufferView(sizeof(VertexData) * kNumVertices_, resource_.Vertex.Get(), static_cast<int>(kNumVertices_));
	resource_.Index = CreateResource::CreateBufferResource(sizeof(uint32_t) * kNumIndices_);
	resource_.IndexBufferView = CreateResource::CreateIndexBufferview(sizeof(uint32_t) * kNumIndices_, resource_.Index.Get());
}


// 更新処理
void Skybox::Update()
{
	// Transformの更新
	transform_.UpdateMatrix();


#ifdef _DEBUG

	if (ImGui::TreeNode("Skybox")) {

		ImGui::DragFloat3("Scale", &transform_.srt.scale.x, 0.1f);
		ImGui::DragFloat3("Rotate", &transform_.srt.rotate.x, 0.1f);
		ImGui::DragFloat3("Translate", &transform_.srt.translate.x, 0.1f);

		ImGui::TreePop();
	}

#endif // _DEBUG
}


// 描画処理
void Skybox::Draw(Camera* camera)
{
	VertexData* vertexData = nullptr;
	Material* material = nullptr;
	uint32_t* indexData = nullptr;


	// 書き込むためにアドレスを取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&material));
	resource_.Index->Map(0, nullptr, reinterpret_cast<void**>(&indexData));


	// 右面。描画インデックスは [0, 1, 2][2, 1, 3]
	vertexData[0].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[1].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[2].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[3].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 2; indexData[4] = 1; indexData[5] = 3;

	// 左面。描画インデックスは [4, 5, 6][6, 5, 7]
	vertexData[4].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[5].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[6].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData[7].position = { -1.0f, -1.0f, 1.0f, 1.0f };

	indexData[6] = 4; indexData[7] = 5; indexData[8] = 6;
	indexData[9] = 6; indexData[10] = 5; indexData[11] = 7;

	// 前面。描画インデックスは [8, 9, 10][10, 9, 11]
	vertexData[8].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[9].position = { 1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[10].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[11].position = { 1.0f, -1.0f, 1.0f, 1.0f };

	indexData[12] = 8; indexData[13] = 9; indexData[14] = 10;
	indexData[15] = 10; indexData[16] = 9; indexData[17] = 11;

	// 後面。描画インデックスは [12, 13, 14][14, 13, 15]
	vertexData[12].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[13].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[14].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertexData[15].position = { -1.0f, -1.0f, -1.0f, 1.0f };

	indexData[18] = 12; indexData[19] = 13; indexData[20] = 14;
	indexData[21] = 14; indexData[22] = 13; indexData[23] = 15;

	// 上面。描画インデックスは [16, 17, 18][18, 17, 19]
	vertexData[16].position = { -1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[17].position = { 1.0f, 1.0f, -1.0f, 1.0f };
	vertexData[18].position = { -1.0f, 1.0f, 1.0f, 1.0f };
	vertexData[19].position = { 1.0f, 1.0f, 1.0f, 1.0f };

	indexData[24] = 16; indexData[25] = 17; indexData[26] = 18;
	indexData[27] = 18; indexData[28] = 17; indexData[29] = 19;

	// 下面。描画インデックスは [20, 21, 22][22, 21, 23]
	vertexData[20].position = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[21].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertexData[22].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertexData[23].position = { 1.0f, -1.0f, -1.0f, 1.0f };

	indexData[30] = 20; indexData[31] = 21; indexData[32] = 22;
	indexData[33] = 22; indexData[34] = 21; indexData[35] = 23;


	// マテリアルの情報を書き込む
	material->color = color_;


	// マップを解除
	resource_.Vertex->Unmap(0, nullptr);
	resource_.Material->Unmap(0, nullptr);
	resource_.Index->Unmap(0, nullptr);


	// コマンドコール
	CommandCall(camera);
}


// コマンドコール
void Skybox::CommandCall(Camera* camera)
{
	// コマンドの取得
	Commands commands = CommandManager::GetInstance()->GetCommands();

	// PipeLineCheck
	PipeLineManager::PipeLineCheckAndSet(PipeLineType::Skybox);

	///// いざ描画！！！！！
	// 頂点の設定
	commands.List->IASetVertexBuffers(0, 1, &resource_.VertexBufferView);
	commands.List->IASetIndexBuffer(&resource_.IndexBufferView);

	// CBVを設定する
	commands.List->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	// wvp用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(1, transform_.constBuffer->GetGPUVirtualAddress());

	// View用のCBufferの場所を設定
	commands.List->SetGraphicsRootConstantBufferView(2, camera->constBuffer->GetGPUVirtualAddress());

	// DescriptorTableを設定する
	if (!texture_ == 0) {
		SRVManager::SetGraphicsRootDescriptorTable(3, texture_);
	}

	// 描画！(DrawCall / ドローコール)。
	commands.List->DrawIndexedInstanced(UINT(kNumIndices_), 1, 0, 0, 0);
}
