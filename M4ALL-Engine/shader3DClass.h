#ifndef _SHADER_3D_CLASS_H_
#define _SHADER_3D_CLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

class Shader3DClass
{
	private:
		struct MatrixBufferType
		{
			D3DXMATRIX world;
			D3DXMATRIX view;
			D3DXMATRIX projection;
		};

		struct LightBufferType
		{
			D3DXVECTOR4 diffuseColor;
			D3DXVECTOR3 lightDirection;
			float padding;  // Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
		};

	public:
		Shader3DClass();
		Shader3DClass(const Shader3DClass&);
		~Shader3DClass();

		bool setup(ID3D11Device* device, HWND windowHandler);
		void destroy();
		bool draw(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, 
			      D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
				  D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);

	private:
		bool setupShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
		void drawShader(ID3D11DeviceContext* deviceContext, int indexCount);
		void destroyShader();
		void outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

		bool setShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, 
								 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture,
								 D3DXVECTOR3 lightDirection, D3DXVECTOR4 diffuseColor);

	private:
		ID3D11VertexShader* vertexShader_;
		ID3D11PixelShader* pixelShader_;
		ID3D11InputLayout* layout_;
		ID3D11SamplerState* sampleState_;
		ID3D11Buffer* matrixBuffer_;
		ID3D11Buffer* lightBuffer_;
};

#endif
