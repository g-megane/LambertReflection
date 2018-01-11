struct PS_INPUT
{
    float4 Pos : SV_POSITION; // 現在のピクセル位置
    float4 PosW : POSITION0;  // オブジェクトのワールド座標
    float4 NorW : NORMAL0;  // 法線
};

// 点光源
struct Light
{
    float4 pos;       // 座標
    float4 diffuse;   // 拡散
    float4 attenuate; // 減衰
};

// マテリアル
struct Material
{
    float4 ambient;  // 環境反射
    float4 diffuse;  // 拡散反射
};

cbuffer ConstantBuffer : register(b0)
{
    float4   eyePos;
    float4   ambient;
    Light    pointLight;
    Material material;
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    float3 n;  // 正規化された法線ベクトル
    float3 l;  // 点光源の方向
    float  d;  // 点光源の距離
    float  a;  // 減衰
    float3 iA; // 環境反射
    float3 iD; // 拡散反射

    // -- ランバート反射モデル --
    n = normalize(input.NorW.xyz);
    l = pointLight.pos.xyz - input.PosW.xyz;
    d = length(l);
    l = normalize(l);
    a = saturate(1.0 / (pointLight.attenuate.x + pointLight.attenuate.y * d + pointLight.attenuate.z * d * d));

    iA = material.ambient.xyz * ambient.xyz;
    iD = saturate(dot(l, n)) * material.diffuse.xyz * pointLight.diffuse.xyz * a;

    return float4(saturate(iA + iD), 1.0);
}