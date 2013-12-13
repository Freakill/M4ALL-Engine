#ifndef _GRAPHICS_MANAGER_H_
#define _GRAPHICS_MANAGER_H_

#if defined(OPENGL)

#else
//Including DirectX libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dx10math.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>

#include "shader3DClass.h"
#include "shader2DClass.h"

class GraphicsManager
{
	public:
		GraphicsManager();
		GraphicsManager(const GraphicsManager&);
		~GraphicsManager();

		bool setup(int width, int height, bool vsync, HWND windowHandler, bool fullscreen, float screenDepthPlane, float screenNearPlane);
		void destroy();

		bool setupShaders(HWND windowHandler);
	
		void draw2D(int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX orthoMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 color);
		void draw3D();

		void beginDraw(float red, float green, float blue, float alpha);
		void endDraw();

	#if defined(OPENGL)

	#else
		ID3D11Device* getDevice();
		ID3D11DeviceContext* getDeviceContext();
		IDXGISwapChain* getSwapChain();

		void getProjectionMatrix(D3DXMATRIX& projectionMatrix);
		void getWorldMatrix(D3DXMATRIX& worldMatrix);
		void getOrthoMatrix(D3DXMATRIX& orthoMatrix);
	#endif

		void getVideoCardInfo(char* cardName, int& memory);

		void turnZBufferOn();
		void turnZBufferOff();

		void turnOnAlphaBlending();
		void turnOffAlphaBlending();

	private:
		bool vsyncEnabled_;
		int videoCardMemory_;
		char videoCardDescription_[128];

		// Pointer to all the variables, interfaces and structures to manage the rendering
	#if defined(OPENGL)

	#else
		IDXGISwapChain* swapChain_;
		ID3D11Device* dx11Device_;
		ID3D11DeviceContext* dx11DeviceContext_;
		ID3D11RenderTargetView* renderTargetView_;
		ID3D11Texture2D* depthStencilBuffer_;
		ID3D11DepthStencilState* depthStencilState_;
		ID3D11DepthStencilView* depthStencilView_;
		ID3D11RasterizerState* rasterState_;
		ID3D11DepthStencilState* depthDisabledStencilState_;
		ID3D11BlendState* alphaEnableBlendingState_;
		ID3D11BlendState* alphaDisableBlendingState_;

		D3DXMATRIX projectionMatrix_;
		D3DXMATRIX worldMatrix_;
		D3DXMATRIX orthoMatrix_;
	#endif

		// Pointers to shaders
		Shader2DClass* shader2D_;
		Shader3DClass* shader3D_;

};

#endif
