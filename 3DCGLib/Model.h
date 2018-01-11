#pragma once
#ifndef MODEL_H
#define MODEL_H
#include "DirectX11.h"
#include "Matrix.h"

namespace Lib
{
    using namespace Microsoft::WRL;

    class Model
    {
    public:
        Model();
        Model(const int SEGMENT);
        ~Model();

        void render(Color &color);

        void setWorldMatrix(Matrix &_world);
        Matrix getWorldMatrix() const;

        Vector3& getLightPos();
        
    private:
        HRESULT init();
        HRESULT initSqhere(const int SEGMENT);
        ComPtr<ID3DBlob> shaderCompile(WCHAR* filename, LPCSTR entryPoint, LPCSTR shaderModel);

        struct SimpleVertex
        {
            float pos[3];
            float normal[3];
        };

        struct ConstantBufferMatrix
        {
            Matrix world;
            Matrix view;
            Matrix projection;
        };

        struct Light
        {
            float pos[4];
            float diffuse[4];
            float specular[4];
            float attenuate[4];
        };

        struct Material
        {
            float ambient[4];
            float diffuse[4];
            float specular[4];
        };

        struct ConstantBufferLight
        {
            float    eyePos[4];
            float    ambient[4];
            Light    pointLight;
            Material material;
        };

        ComPtr<ID3D11VertexShader>     vertexShader;
        ComPtr<ID3D11PixelShader>      pixelShader;
        ComPtr<ID3D11InputLayout>      vertexLayout;
        ComPtr<ID3D11Buffer>           vertexBuffer;
        ComPtr<ID3D11Buffer>           indexBuffer;
        ComPtr<ID3D11Buffer>           constantBufferMatrix;
        ComPtr<ID3D11Buffer>           constantBufferLight;

        Matrix world;
        Vector3 light;
        int vertexCount;
    };
}
#endif
