#ifndef _SHADER_2D_CLASS_H_
#define _SHADER_2D_CLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

class Shader2DClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct PixelBufferType
	{
		D3DXVECTOR4 pixelColor;
	};

public:
	Shader2DClass();
	Shader2DClass(const Shader2DClass&);
	~Shader2DClass();

	bool setup(ID3D11Device*, HWND);
	bool draw(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);
	void destroy();

private:
	bool setupShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void drawShader(ID3D11DeviceContext*, int);
	void destroyShader();
	void outputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool setShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, D3DXVECTOR4);

private:
	ID3D11VertexShader* vertexShader_;
	ID3D11PixelShader* pixelShader_;
	ID3D11InputLayout* layout_;
	ID3D11Buffer* matrixBuffer_;
	ID3D11SamplerState* sampleState_;
	ID3D11Buffer* pixelBuffer_;
};

#endif