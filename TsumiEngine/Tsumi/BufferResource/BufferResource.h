#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgidebug.h>
#include <dxcapi.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxcompiler.lib")

#include <assert.h>

#include "../Base/DXCommon/DirectXCommon.h"
#include "../CommandManager/CommandManager.h"
#include "../../Project/Math/MyMath.h"
#include "../../Project/Math/Struct.h"


/* BufferResourceクラス */
template<typename T>
class BufferResource {

public:

	// コンストラクタ、デストラクタ
	BufferResource() {};
	~BufferResource() {};


	// Resource作成
	void CreateResource(UINT itemCount = 1);

	// VertexBufferViewの作成
	void CreateVertexBufferView();

	// IndexBufferViewの作成
	void CreateIndexBufferView();

	// ResourceをマップしてCPUアクセスを可能にする
	void Map();

	// Resourceのマップを解除してGPUアクセスを解除する
	void UnMap();

	// データを書き込む
	void WriteData(const T data);

	// コマンドを積む
	void CommandCall(UINT number);


private:

	// BufferResourceの生成
	void CreateBufferResource();


private: 

	// Resource
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;

	// VertexBufferView
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
	// IndexBufferView
	D3D12_INDEX_BUFFER_VIEW indexBufferView_;

	// 作成するResourceの要素数
	UINT itemCount_ = 1;

	// mappedData
	T* mappedData_{};

};


// Resourceの作成
template<typename T>
inline void BufferResource<T>::CreateResource(UINT itemCount) 
{
	// 作成するResourceの要素数
	this->itemCount_ = itemCount;

	// BufferResourceの作成
	CreateBufferResource();
}


// VertexBufferViewの作成
template<typename T>
inline void BufferResource<T>::CreateVertexBufferView()
{
	vertexBufferView_.BufferLocation = buffer_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = UINT(sizeof(T) * itemCount_);
	vertexBufferView_.StrideInBytes = UINT(sizeof(T));
}


// IndexBufferViewの作成
template<typename T>
inline void BufferResource<T>::CreateIndexBufferView()
{
	indexBufferView_.BufferLocation = buffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(T) * itemCount_);
	indexBufferView_.SizeInBytes = UINS(sizeof(T));
}


// ResourceをマップしてCPUアクセスを可能にする
template<typename T>
inline void BufferResource<T>::Map()
{
	// buffer_ が nullptr の場合はエラー
	if (!buffer_) {
		// ログを出力し、アサーションでプログラムを停止させる
		Log("constBuff_ is nullptr. Make sure to create constBuffer before calling Map.");
		assert(false);
		return;
	}

	HRESULT result;
	result = buffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
	assert(SUCCEEDED(result));
}


// Resourceのマップを解除してGPUアクセスを解除する
template<typename T>
inline void BufferResource<T>::UnMap()
{
	// buffer_ と mappedData_ が有効な時にUnMap
	if (buffer_ && mappedData_) {
		buffer_->Unmap(0, nullptr);
		mappedData_ = nullptr;
	}
}


// データを書き込む
template<typename T>
inline void BufferResource<T>::WriteData(const T data)
{
	assert(mappedData_);
	*mappedData_ = data;
}


// コマンドを積む
template<typename T>
inline void BufferResource<T>::CommandCall(UINT number)
{
	Commands commands = CommandManager::GetInstance()->GetCommands();
	commands.List->SetGraphicsRootConstantBufferView(number, buffer_->GetGPUVirtualAddress());
}


// BufferResourceの生成
template<typename T>
inline void BufferResource<T>::CreateBufferResource()
{
	// Resource用のHeap設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties_{};
	uploadHeapProperties_.Type = D3D12_HEAP_TYPE_UPLOAD; // UploadHeapを使う

	// BufferResource。Textureの場合はまた別の設定をする
	D3D12_RESOURCE_DESC vertexResourceDesc_{};
	vertexResourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	// Resourceのサイズ
	vertexResourceDesc_.Width = sizeof(T) * itemCount_;

	// Bufferの場合はこれらは1にする決まり
	vertexResourceDesc_.Height = 1;
	vertexResourceDesc_.DepthOrArraySize = 1;
	vertexResourceDesc_.MipLevels = 1;
	vertexResourceDesc_.SampleDesc.Count = 1;

	// Bufferの場合はこれにする決まり
	vertexResourceDesc_.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 実際にBufferResourceを作る
	HRESULT hr_;
	hr_ = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties_, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc_, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&buffer_));
	assert(SUCCEEDED(hr_));

}
