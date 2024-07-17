#pragma once

#include "../../../Project/Math/MyMath.h"
#include "../../../Project/Math/Struct.h"
#include "../../CreateResource/CreateResource.h"

#include <vector>



// 定数バッファー構造体
struct TransformationMatrix {
	Matrix4x4 World;
	Matrix4x4 WorldInverseTranspose;
};
struct SRT {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
	SRT() : scale(1.0f, 1.0f, 1.0f), rotate(0.0f, 0.0f, 0.0f), translate(0.0f, 0.0f, 0.0f) {}
};


/// <summary>
/// ワールド変換データ
/// </summary>
struct WorldTransform {

	//// ローカルスケール
	//Vector3 scale = Vector3::one;
	//// ローカル回転軸
	//Vector3 rotate = Vector3::zero;
	//// ローカル座標
	//Vector3 translate = Vector3::zero;

	SRT srt{};

	// ローカル -> ワールド変換行列
	Matrix4x4 matWorld{};
	Matrix4x4 WorldInverseTranspose{};

	// ペアレント
	const WorldTransform* parent{};

	// 定数バッファー
	ComPtr<ID3D12Resource> constBuffer = nullptr;

	// マッピング済みアドレス
	TransformationMatrix* constMap;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	/*WorldTransform()
		: srt{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} }
	{}*/
	WorldTransform()
		: srt{}
	{}

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 行列の更新処理
	/// </summary>
	void UpdateMatrix();

#pragma region SRTアクセスメソッド

	// アクセス用メソッドを定義
	Vector3& scale() { return srt.scale; }
	Vector3& rotate() { return srt.rotate; }
	Vector3& translate() { return srt.translate; }

	const Vector3& scale() const { return srt.scale; }
	const Vector3& rotate() const { return srt.rotate; }
	const Vector3& translate() const { return srt.translate; }

#pragma endregion 

#pragma region Get

	/// <summary>
	/// ワールド座標の取得
	/// </summary>
	Vector3 GetWorldPos();

#pragma endregion

#pragma region Set

	/// <summary>
	/// 親子関係を結ぶ
	/// </summary>
	void SetParent(const WorldTransform* parentTransform);

#pragma endregion


	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateBuffer();

	/// <summary>
	/// マッピングする
	/// </summary>
	void Map();

	/// <summary>
	/// マッピング終了
	/// </summary>
	void UnMap();

	/// <summary>
	/// 行列の計算・転送
	/// </summary>
	void TransferMatrix();
};
