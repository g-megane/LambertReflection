struct PS_INPUT
{
    float4 Pos : SV_POSITION; // ���݂̃s�N�Z���ʒu
    float4 PosW : POSITION0;  // �I�u�W�F�N�g�̃��[���h���W
    float4 NorW : NORMAL0;  // �@��
};

// �_����
struct Light
{
    float4 pos;       // ���W
    float4 diffuse;   // �g�U
    float4 attenuate; // ����
};

// �}�e���A��
struct Material
{
    float4 ambient;  // ������
    float4 diffuse;  // �g�U����
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
    float3 n;  // ���K�����ꂽ�@���x�N�g��
    float3 l;  // �_�����̕���
    float  d;  // �_�����̋���
    float  a;  // ����
    float3 iA; // ������
    float3 iD; // �g�U����

    // -- �����o�[�g���˃��f�� --
    n = normalize(input.NorW.xyz);
    l = pointLight.pos.xyz - input.PosW.xyz;
    d = length(l);
    l = normalize(l);
    a = saturate(1.0 / (pointLight.attenuate.x + pointLight.attenuate.y * d + pointLight.attenuate.z * d * d));

    iA = material.ambient.xyz * ambient.xyz;
    iD = saturate(dot(l, n)) * material.diffuse.xyz * pointLight.diffuse.xyz * a;

    return float4(saturate(iA + iD), 1.0);
}