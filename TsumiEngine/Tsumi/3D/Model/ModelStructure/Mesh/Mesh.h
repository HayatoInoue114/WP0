#pragma once

#include "../../../Project/Math/MyMath.h"
#include "../Material/Material.h"


// モデルデータ
struct MeshData {
	std::vector<VertexData> vertices;
	Node rootNode;
};


/* Mesh構造体 */
struct Mesh {

public:

	// コンストラクタ
	Mesh() = default;

	// メッシュ作成
	void Create();

	// バッファ作成
	void CreateBuffer();

	// マッピングする
	void Map();

	// マッピング終了
	void UnMap();

	// メッシュの転送
	void TransferMesh();


public:

	// メッシュ名
	string name;

	// バーテックスデータ
	MeshData meshData{};

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

	// マップアドレス
	MeshData* constMap;
};

