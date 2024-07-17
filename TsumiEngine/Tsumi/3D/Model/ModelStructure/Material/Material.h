#pragma once

#include "../../../Project/Math/MyMath.h"
#include "../../../Project/Math/Struct.h"

using namespace std;

struct MaterialDataN {
	Vector4 color;
	Matrix4x4 uvTransform;
};


/* Material構造体 */
struct MaterialModel {

	// コンストラクタ
	MaterialModel() = default;
	MaterialModel(const string& mtlName) {
		Create();
		name = mtlName;
	}

	// マテリアル作成
	void Create();

	// バッファ作成
	void CreateBuffer();

	// マッピングする
	void Map();

	// マッピング終了
	void UnMap();

	// マテリアルの転送
	void TransferMaterial();


	// マテリアル名
	string name;

	// テクスチャハンドル
	uint32_t textureHandle = 1u;

	// 定数バッファ構造体
	MaterialDataN materialData{};

	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffer = nullptr;

	// マップアドレス
	MaterialDataN* constMap{};
};

