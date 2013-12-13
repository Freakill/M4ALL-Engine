#ifndef _IMAGE_CLASS_H_
#define _IMAGE_CLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include "textureclass.h"

class ImageClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

public:
	ImageClass();
	ImageClass(const ImageClass&);
	~ImageClass();

	bool setup(ID3D11Device*, int, int, const std::string&, int, int);
	bool update(ID3D11DeviceContext*, int, int);
	bool draw(ID3D11DeviceContext*, int, int);
	void destroy();

	int getIndexCount();
	ID3D11ShaderResourceView* getTexture();

private:
	bool setupBuffers(ID3D11Device*);
	bool updateBuffers(ID3D11DeviceContext*, int, int);
	void drawBuffers(ID3D11DeviceContext*);
	void destroyBuffers();

	bool loadTexture(ID3D11Device*, const std::string&);
	void releaseTexture();

private:
	ID3D11Buffer *vertexBuffer_, *indexBuffer_;
	int vertexCount_, indexCount_;
	TextureClass* texture_;
	int screenWidth_, screenHeight_;
	int bitmapWidth_, bitmapHeight_;
	int previousPosX_, previousPosY_;
};

#endif