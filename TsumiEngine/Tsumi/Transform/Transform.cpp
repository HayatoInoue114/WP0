#include "Transform.h"



// 初期化処理
void Transform::Initialize()
{
	CreateBuffer();
	matWorld = Matrix4x4::identity;
	UpdateMatrix();
}


// 行列の更新処理
void Transform::UpdateMatrix()
{
	// スケール、回転、並列移動を合成して行列を計算する
	matWorld = MakeAffineMatrix(scale, rotate, translate);

	// 親があれば親のワールド行列を掛ける
	if (parent) {
		matWorld *= parent->matWorld;
	}

	// 行列の計算・転送
	TransferMatrix();
}


// ワールド座標の取得
Vector3 Transform::GetWorldPos()
{
	return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };
}


// 親子関係を結ぶ
void Transform::SetParent(const Transform* parentTransform)
{
	parent = parentTransform;
}


// 定数バッファの生成
void Transform::CreateBuffer()
{
	CreateResource::CreateBufferResource(sizeof(TransformationMat), constBuffer);

	// constBuffer が nullptr の場合はエラーログを出力してアサーションでプログラムを停止させる
	if (!constBuffer) {
		assert(false && "Failed to create constBuffer.");
	}
}


// マッピングする
void Transform::Map()
{
	// constBuffer が nullptr の場合はエラーハンドリング
	if (!constBuffer) {
		// エラーログなどを出力するか、適切な対処を行う
		// この例ではエラーログを出力し、アサーションでプログラムを停止させる
		assert(false && "constBuff_ is nullptr. Make sure to create constBuffer before calling Map.");
		return;
	}

	HRESULT result;
	result = constBuffer->Map(0, nullptr, reinterpret_cast<void**>(&constMap));
	assert(SUCCEEDED(result));
}


// マッピング終了
void Transform::UnMap()
{
	constBuffer->Unmap(0, nullptr);
}


// 行列の計算・転送
void Transform::TransferMatrix()
{
	Map();
	constMap->World = World;
	constMap->WVP = WVP;
	constMap->WorldInverseTranspose = WorldInverseTranspose;
	UnMap();
}
