#include "Object3d.hlsli"
#include "MathFunc.hlsli"


struct Material
{
    float4 color;
    float4x4 uvTransform;
};
struct ViewProjectionMatrix
{
    float4x4 view;
    float4x4 projection;
    float4x4 cameraPos;
};
struct DirectionalLight
{
    float4 color; // 色(RGBA)
    float3 direction; // ライトの向き
    float intensity; // 輝度
};
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<ViewProjectionMatrix> gViewProjectionMat : register(b1);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float4(input.texCoord, 0.0f, 1.0f), gMaterial.uvTransform);

    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
    // 色が不透明な場合は描画する
    if (textureColor.a < 0.9f) // アルファ値の閾値を変更
    {
        discard;
    }
    
    output.color.rgb = textureColor.rgb;
    output.color.a = gMaterial.color.a * textureColor.a; // 色の透明度の計算

    return output;
}