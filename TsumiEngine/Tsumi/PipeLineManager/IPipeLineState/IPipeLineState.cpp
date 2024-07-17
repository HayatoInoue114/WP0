#include "IPipeLineState.h"
#include "../../ShaderManager/ShaderManager.h"




// InputLayoutのセットアップ
D3D12_INPUT_ELEMENT_DESC IPipeLineState::SetUpInputElementDescs(LPCSTR SemanticName)
{
	D3D12_INPUT_ELEMENT_DESC inputElementDescs{};

	if (strcmp(SemanticName, "POSITION") == 0)
	{
		inputElementDescs.SemanticName = "POSITION";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "TEXCOORD") == 0) 
	{
		inputElementDescs.SemanticName = "TEXCOORD";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "NORMAL") == 0)
	{
		inputElementDescs.SemanticName = "NORMAL";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "WORLDPOSITION") == 0)
	{
		inputElementDescs.SemanticName = "WORLDPOSITION";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "CAMERAPOSITION") == 0)
	{
		inputElementDescs.SemanticName = "CAMERAPOSITION";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "COLOR") == 0)
	{
		inputElementDescs.SemanticName = "COLOR";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 0;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "WEIGHT") == 0)
	{
		inputElementDescs.SemanticName = "WEIGHT";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs.InputSlot = 1;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}
	else if (strcmp(SemanticName, "INDEX") == 0)
	{
		inputElementDescs.SemanticName = "INDEX";
		inputElementDescs.SemanticIndex = 0;
		inputElementDescs.Format = DXGI_FORMAT_R32G32B32A32_SINT;
		inputElementDescs.InputSlot = 1;
		inputElementDescs.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	}

	return inputElementDescs;
}
void IPipeLineState::SetUpInputLayout(D3D12_INPUT_LAYOUT_DESC& inputLayoutDesc, const D3D12_INPUT_ELEMENT_DESC* inputElementDescs, UINT numInputElements)
{
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = numInputElements;
}


// BlendStateのセットアップ
void IPipeLineState::SetUpBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode)
{
	blendDesc.RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.BlendEnable = true;


	switch (blendMode)
	{
	case None:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;

	case Add:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case Subtruct:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	case Multiply:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_ZERO;
		blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;
		break;

	case Screen:
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blendDesc.DestBlend = D3D12_BLEND_ONE;
		break;

	default:
		break;
	}
}


// RasterizerStateのセットアップ
void IPipeLineState::SetUpRasterizerState(D3D12_RASTERIZER_DESC& rasterizerDesc, D3D12_CULL_MODE Cull, D3D12_FILL_MODE fill)
{
	// 裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = Cull;
	// 三角形の中を塗りつぶす
	rasterizerDesc.FillMode = fill;
}


// Shadersのコンパイル
void IPipeLineState::SetUpModelShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key)
{
	vertexShaderBlob = ShaderManager::GetInstance()->GetModelShader(key).VertexBlob;
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = ShaderManager::GetInstance()->GetModelShader(key).PixelBlob;
	assert(pixelShaderBlob != nullptr);
}
void IPipeLineState::SetUpPostEffectShader(IDxcBlob*& vertexShaderBlob, IDxcBlob*& pixelShaderBlob, const std::string key)
{
	vertexShaderBlob = ShaderManager::GetInstance()->GetPostEffectShader(key).VertexBlob;
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob = ShaderManager::GetInstance()->GetPostEffectShader(key).PixelBlob;
	assert(pixelShaderBlob != nullptr);
}

