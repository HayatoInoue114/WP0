#pragma once

#include <iostream>
#include <memory>

#include "../../BufferResource/BufferResource.h"
#include "../../CommandManager/CommandManager.h"
#include "../../View/SRVManager/SRVManager.h"
#include "../../View/RTVManager/RTVManager.h"
#include "../../PipeLineManager/PipeLineManager.h"
#include "../../TextureManager/TextureManager.h"
#include "../Materials/PostEffectMaterials.h"
#include "../../Project/GameObject/Camera/Camera.h"
#include "../../../Project/Math/MyMath.h"


/* IPostEffectクラス */
template <typename T>
class IPostEffect {

public: // メンバ関数

	// デストラクタ
	virtual ~IPostEffect() {};

	// 初期化処理、更新処理、描画処理
	virtual void Initialize() = 0;
	virtual void Draw([[maybe_unused]] Camera* camera) = 0;

	// ImGuiの描画
	virtual void DrawImGui(std::string name = "") = 0;


protected:

	// コマンドコール
	virtual void CommandCall() = 0;

	// SRVやBufferの作成
	virtual void Create() {
		// SRV作成
		Microsoft::WRL::ComPtr<ID3D12Resource> stv = 
			RTVManager::GetRTV("PostEffect")->GetRTVPrope().Resources.Get();
		srv_ = SRVManager::CreatePostEffectSRV(stv);

		// バッファー作成
		mtlBuffer_.CreateResource();
	}


protected: // メンバ変数

	// SRV
	uint32_t srv_ = 0;

	// バッファーにセットするMaterialData
	T mtlData_{};

	// バッファー
	BufferResource<T> mtlBuffer_{};


	// ImGuiLabel 
	std::string label_;
};

