cbuffer ConstantBuffer : register(b0)
{
    matrix World;           // ワールド行列
    matrix View;            // ビュー行列
    matrix Projection;      // 射影行列
    float4 lightAmbient;    // 環境光
    float4 materialAmbient; // 物体の色
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // 現在のピクセル位置
    float4 PosW : POSITION0;  // オブジェクトのワールド座標
    float4 NorW : TEXCOORD0;  // 法線
};

float4 PS(PS_INPUT input) : SV_TARGET
{
    float3 i;

    i = materialAmbient.xyz * lightAmbient.xyz;

    return float4(i, 1.0);
}