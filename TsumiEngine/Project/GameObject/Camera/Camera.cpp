#include "Camera.h"



// 初期化処理
void Camera::Initialize(Vector3 initRotate, Vector3 initTranslate)
{
	rotate = initRotate;
	translate = initTranslate;

	CreateBuffer();
	Map();
	UpdateMatrix();
}


// 行列の更新処理
void Camera::UpdateMatrix()
{
	// 回転行列を作成
	rotateMat = MakeRotateXYZMatrix(rotate);

	// 平行移動行列を作成
	translateMat = MakeTranslateMatrix(translate);

	// ワールド行列を作成
	matWorld = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);

	// ビュー行列を作成（カメラの位置と向きを基に逆行列を計算）
	viewMatrix = Inverse(matWorld);

	// 投影行列を作成（透視投影）
	projectionMatrix = MakePerspectiveFovMatrix(fov, aspectRatio, nearZ, farZ);

	// 投影逆行列
	projectionInverseMatrix = Inverse(projectionMatrix);

	// 正射影行列を作成（ウィンドウサイズを基に設定）
	orthoGraphicMatrix = MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 100.0f);

	// ビューポート行列を作成（ウィンドウサイズを基に設定）
	viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);

	// ビュープロジェクション行列
	viewProjectionMatrix = projectionMatrix * viewMatrix;

	// ビュー・投影・ビューポート行列を掛け合わせた行列を作成
	viewProjectionViewportMatrix = viewportMatrix * viewProjectionMatrix;

	// ビュー・投影・ビューポート行列の逆行列を作成
	inverseViewProjectionViewportMatrix = Inverse(viewProjectionViewportMatrix);

	TransferMatrix();
}


// ワールド座標の取得
Vector3 Camera::GetWorldPos()
{
	return { matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2] };
}


// 定数バッファの生成
void Camera::CreateBuffer()
{
	CreateResource::CreateBufferResource(sizeof(TransformationViewMatrix), constBuffer);

	// constBuffer が nullptr の場合はエラーログを出力してアサーションでプログラムを停止させる
	if (!constBuffer) {
		assert(false && "Failed to create constBuffer.");
	}
}


// マッピングする
void Camera::Map()
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
void Camera::UnMap()
{
	constBuffer->Unmap(0, nullptr);
}


// 行列の転送
void Camera::TransferMatrix()
{
	Map();

	constMap->view = viewMatrix;
	constMap->viewProjection = projectionMatrix;
	constMap->orthoGraphic = orthoGraphicMatrix;
	constMap->cameraPosition = translate;

	UnMap();
}
