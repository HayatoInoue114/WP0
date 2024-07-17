#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>

// PSO
struct PsoProperty {
	ID3D12PipelineState* graphicsPipelineState = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopologyType;
};

// BlendMode
enum  BlendMode {
	None,
	Add,
	Subtruct,
	Multiply,
	Screen,
};
