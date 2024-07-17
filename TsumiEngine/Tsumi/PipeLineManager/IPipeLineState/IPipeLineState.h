#pragma once

#include <d3dx12.h>
#include <dxcapi.h>

#include <map>

#include "../../../Project/Math/MyMath.h"
#include "../../../Project/Math/Struct.h"
#include "../../Base/DXCommon/DirectXCommon.h"
#include "../PipeLineStructure/PipeLineStructure.h"
#include "../../ShaderManager/ShaderManager.h"


/* IPipeLineStateクラス */
class IPipeLineState {

public: // メンバ関数

	// デストラクタ
	~IPipeLineState() = default;

	// Psoを構築する
	virtual PsoProperty SetUpPso() = 0;

#pragma region Accessor アクセッサ

	// Psoの取得
	PsoProperty GetPsoProperty() const { return pso_; }

#pragma endregion


protected: // メンバ関数

	// RootSignatureのセットアップ
	virtual void SetUpRootSignature(D3D12_ROOT_SIGNATURE_DESC& descriptionRootSignature) = 0;

	// InputLayoutのセットアップ
	D3D12_INPUT_ELEMENT_DESC SetUpInputElementDescs(LPCSTR SemanticName);
	void SetUpInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements);

	// BlendStateのセットアップ
	void SetUpBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode);

	// RasterizerStateのセットアップ
	void SetUpRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc, D3D12_CULL_MODE Cull, D3D12_FILL_MODE fill);

	// Shadersのコンパイル
	void SetUpModelShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key);
	void SetUpPostEffectShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key);


protected: // メンバ変数

	PsoProperty pso_{};
};

