#include "ShaderManager.h"



/// <summary>
/// 初期化処理
/// </summary>
void ShaderManager::Initialize() {

	// DXCの初期化
	ShaderManager::GetInstance()->InitializeDXC();
	// includeに対応するための設定 
	ShaderManager::GetInstance()->SettingIncludeHandler();
	// シェーダーをコンパイルする
	ShaderManager::GetInstance()->ModelShadersCompiles();
	ShaderManager::GetInstance()->PostEffectShadersCompiles();
}



/// <summary>
/// CompileShader関数
/// </summary>
IDxcBlob* ShaderManager::CompileShader(const std::wstring& filePath, const wchar_t* profile) {

	IDxcUtils* dxcUtils = ShaderManager::GetInstance()->dxc_.Utils;
	IDxcCompiler3* dxcCompiler = ShaderManager::GetInstance()->dxc_.Compiler;
	IDxcIncludeHandler* includeHandler = ShaderManager::GetInstance()->dxc_.includeHandler;
 
	/* --- 1. hlslファイルを読む --- */

	// これからシェーダーをコンパイルする旨をログに出す
	Log(ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	// hlslを読み込む
	HRESULT hr_ = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読み込めなかったら止める
	assert(SUCCEEDED(hr_));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; // UTF8の文字コードであることを通知


	/* --- 2.Compilerする --- */

	LPCWSTR arguments[] = {
		filePath.c_str(),  // コンパイル対象のhlslファイル名
		L"-E", L"main",  // エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile,  // ShaderProfileの設定
		L"-Zi", L"-Qembed_debug",  // デバッグ用の情報を埋め込む
		L"-Od",  // 最適化を外しておく
		L"-Zpr",  // メモリレイアウトは行優先
	};
	// 実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr_ = dxcCompiler->Compile(
		&shaderSourceBuffer,        // 読み込んだファイル
		arguments,					// コンパイルオプション
		_countof(arguments),		// コンパイルオプションの数
		includeHandler,				// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)	// コンパイル結果
	);
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr_));


	/* --- 3. 警告・エラーが出てないか確認する --- */

	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		// 警告・エラーゼッタイ
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	/* --- 4. Compiler結果を受け取って返す --- */

	// コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr_ = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr_));
	// 成功したログを出す
	Log(ConvertString(std::format(L"Compile Succeded, path{}, profile:{}\n", filePath, profile)));
	// もう使わないリソースを解放
	shaderResult->Release();
	shaderSource->Release();
	// 実行用のバイナリを返却
	return shaderBlob;
}



/// <summary>
/// DXCの初期化
/// </summary>
void ShaderManager::InitializeDXC() {

	HRESULT hr{};

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&ShaderManager::GetInstance()->dxc_.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&ShaderManager::GetInstance()->dxc_.Compiler));
	assert(SUCCEEDED(hr));
}



/// <summary>
/// includeに対応するための設定
/// </summary>
void ShaderManager::SettingIncludeHandler() {

	ShaderManager::GetInstance()->dxc_.Utils->CreateDefaultIncludeHandler(&ShaderManager::GetInstance()->dxc_.includeHandler);
}



/// <summary>
/// シェーダーをコンパイルする
/// </summary>
void ShaderManager::ModelShadersCompiles()
{
	NormalShader();
	SpriteShader();
	LightShader();
	LambertShader();
	PhongShader();
	PhongNormalMapShader();
	ParticleShader();
	LineShader();
	Object3DShader();
	SkinningObject3dShader();
	SkyboxShader();
}
void ShaderManager::PostEffectShadersCompiles()
{
	PostEffectShader();
	AbsentShader();
	BoxFilterShader();
	//ColorGrading();
	DissolveShader();
	GaussianFilterShader();
	GrainShader();
	GrayScaleShader();
	OutLineShader();
	RadialBlurShader();
	RandomShader();
	SepiaToneShader();
	VignettingShader();
}



/// <summary>
/// シェーダーをセットする
/// </summary>
void ShaderManager::SetShader(const std::wstring& vertexPath, const std::wstring& pixelPath, ShadersMode& shader)
{
	shader.VertexBlob = ShaderManager::CompileShader(vertexPath.c_str(), L"vs_6_0");
	shader.PixelBlob = ShaderManager::CompileShader(pixelPath.c_str(), L"ps_6_0");
}



void ShaderManager::NormalShader() {

	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/NormalObject3d.VS.hlsl", 
		L"Resources/Shaders/NormalObject3d.PS.hlsl", 
		shader);
	modelShadersMap_["Normal"] = shader;
}

void ShaderManager::SpriteShader() {

	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/SpriteObject3d.VS.hlsl",
		L"Resources/Shaders/SpriteObject3d.PS.hlsl",
		shader);
	modelShadersMap_["Sprite"] = shader;
}

void ShaderManager::LightShader() {

	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/LightObject.VS.hlsl",
		L"Resources/Shaders/LightObject.PS.hlsl",
		shader);
	modelShadersMap_["Light"] = shader;
}

void ShaderManager::LambertShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/LambertObject.VS.hlsl",
		L"Resources/Shaders/LambertObject.PS.hlsl",
		shader);
	modelShadersMap_["Lambert"] = shader;
}

void ShaderManager::PhongShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PhongObject.VS.hlsl",
		L"Resources/Shaders/PhongObject.PS.hlsl",
		shader);
	modelShadersMap_["Phong"] = shader;
}

void ShaderManager::PhongNormalMapShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PhongNormalMap.VS.hlsl",
		L"Resources/Shaders/PhongNormalMap.PS.hlsl",
		shader);
	modelShadersMap_["PhongNormalMap"] = shader;
}

void ShaderManager::ParticleShader() {

	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/ParticleObject3D.VS.hlsl",
		L"Resources/Shaders/ParticleObject3D.PS.hlsl",
		shader);
	modelShadersMap_["Particle"] = shader;
}

void ShaderManager::LineShader() {

	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/LineObject3d.VS.hlsl",
		L"Resources/Shaders/LineObject3d.PS.hlsl",
		shader);
	modelShadersMap_["Line"] = shader;
}

void ShaderManager::Object3DShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/Object3d.VS.hlsl",
		L"Resources/Shaders/Object3d.PS.hlsl",
		shader);
	modelShadersMap_["Object3D"] = shader;
}

void ShaderManager::SkinningObject3dShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/SkinningObject3d.VS.hlsl",
		L"Resources/Shaders/SkinningObject3d.PS.hlsl",
		shader);
	modelShadersMap_["SkinningObject3d"] = shader;
}

void ShaderManager::SkyboxShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/Skybox/Skybox.VS.hlsl",
		L"Resources/Shaders/Skybox/Skybox.PS.hlsl",
		shader);
	modelShadersMap_["Skybox"] = shader;
}



void ShaderManager::PostEffectShader() {

	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect.PS.hlsl",
		shader);
	postEffectShadersMap_["PostEffect"] = shader;
}

void ShaderManager::AbsentShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/Absent/Absent.PS.hlsl",
		shader);
	postEffectShadersMap_["Absent"] = shader;
}

void ShaderManager::BoxFilterShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/BoxFilter/BoxFilter.PS.hlsl",
		shader);
	postEffectShadersMap_["BoxFilter"] = shader;
}

void ShaderManager::ColorGrading()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/ColorGrading/ColorGrading.PS.hlsl",
		shader);
	postEffectShadersMap_["ColorGrading"] = shader;
}

void ShaderManager::DissolveShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/Dissolve/Dissolve.PS.hlsl",
		shader);
	postEffectShadersMap_["Dissolve"] = shader;
}

void ShaderManager::GaussianFilterShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/GaussianFilter/GaussianFilter.PS.hlsl",
		shader);
	postEffectShadersMap_["GaussianFilter"] = shader;
}

void ShaderManager::GrainShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/Grain/Grain.PS.hlsl",
		shader);
	postEffectShadersMap_["Grain"] = shader;
}

void ShaderManager::GrayScaleShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/GrayScale/GrayScale.PS.hlsl",
		shader);
	postEffectShadersMap_["GrayScale"] = shader;
}

void ShaderManager::OutLineShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/OutLine/OutLine.PS.hlsl",
		shader);
	postEffectShadersMap_["OutLine"] = shader;
}

void ShaderManager::RadialBlurShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/RadialBlur/RadialBlur.PS.hlsl",
		shader);
	postEffectShadersMap_["RadialBlur"] = shader;
}

void ShaderManager::RandomShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/Random/Random.PS.hlsl",
		shader);
	postEffectShadersMap_["Random"] = shader;
}

void ShaderManager::SepiaToneShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/SepiaTone/SepiaTone.PS.hlsl",
		shader);
	postEffectShadersMap_["SepiaTone"] = shader;
}

void ShaderManager::VignettingShader()
{
	ShadersMode shader{};
	SetShader(
		L"Resources/Shaders/PostEffect.VS.hlsl",
		L"Resources/Shaders/PostEffect/Vignetting/Vignetting.PS.hlsl",
		shader);
	postEffectShadersMap_["Vignetting"] = shader;
}
