#pragma once

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "../../../../../Project/Math/MyMath.h"
#include "../../../../../Project/Math/Struct.h"

#include <cassert>


/* ObjDataResourceクラス */
class ObjDataResource {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjDataResource(ModelData objData, uint32_t index);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ObjDataResource() {};


#pragma region Get

	/// <summary>
	/// Objデータ
	/// </summary>
	ModelData GetObjData() { return objData_; }

	/// <summary>
	/// テクスチャハンドルの取得
	/// </summary>
	uint32_t GetObjHandle() const { return index_; }

#pragma endregion 


private:

	ModelData objData_{};
	uint32_t index_;

};