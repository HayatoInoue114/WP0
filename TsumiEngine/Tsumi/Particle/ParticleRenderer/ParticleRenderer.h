#pragma once

#include "../../Base/DXCommon/DirectXCommon.h"
#include "../../../Project/Math/MyMath.h"
#include "../../../Project/Math/Struct.h"
#include "../IParticle.h"
#include "../../PipeLineManager/PipeLineManager.h"
#include "../../CreateResource/CreateResource.h"
#include "../../DescriptorManager/DescriptorManager.h"
#include "../../View/SRVManager/SRVManager.h"
#include "../../../Project/GameObject/Camera/Camera.h"


/* ParticleRendererクラス */
class ParticleRenderer {

public: // メンバ関数

	// コンストラクタとデストラクタ
	ParticleRenderer() {};
	~ParticleRenderer() {};

	// 初期化処理、更新処理、描画処理
	void Initialize(uint32_t maxInstance);
	void Draw(const list<unique_ptr<IParticle>>& p, Camera* camera);

#pragma region Get 取得

	// テクスチャの取得
	uint32_t GetUseTexture() { return this->useTexture_; }

#pragma endregion 

#pragma region Set 取得

	// テクスチャの設定
	void SetUseTexture(uint32_t setTexture) { this->useTexture_ = setTexture; }

#pragma endregion 


private: // メンバ関数

	// コマンドコール
	void CommandCall();

	// ビルボードの処理
	Matrix4x4 CalcBillBord(Camera* camera);


private: // メンバ変数

	// リソース
	ResourcePeroperty resource_{};

	// 
	uint32_t dsvIndex_ = 0;

	// インスタンスの最大と個数
	const uint32_t kMaxInstanceNum_ = 5000;
	uint32_t instanceNum_;


	Vector4 pos_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	float size_ = 0.5f;

	// 使用するテクスチャ
	uint32_t useTexture_;
};

