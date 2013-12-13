#ifndef _MODEL_CLASS_H_
#define _MODEL_CLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include <Windows.h>

#include "cal3d/cal3d.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "pugiconfig.h"
#include "pugixml.h"

#include "textureClass.h"

class ModelClass
{
	private:
		struct VertexType
		{
			D3DXVECTOR3 position;
			D3DXVECTOR3 normal;
			float tu, tv;
		};

	public:
		ModelClass();
		ModelClass(const ModelClass&);
		~ModelClass();

		bool setup(ID3D11Device*, char*);
		void update(float elapsedTime);
		void draw(ID3D11Device*, ID3D11DeviceContext*);
		void destroy();

		int getIndexCount();

		ID3D11ShaderResourceView* getTexture();

		void decreaseAnimationToDisplay();
		void increaseAnimationToDisplay();

	private:
		bool setupBuffers(ID3D11Device*);
		bool updateBuffers(ID3D11DeviceContext*);
		void drawBuffers(ID3D11DeviceContext*);
		void destroyBuffers();

		bool loadTexture(ID3D11Device*, const std::string& strFilename);
		void destroyTexture();

		bool loadModel(char*);
		void destroyModel();

		bool parseBasicModelConfiguration(const std::string& strFilename);

		ID3D11Buffer *vertexBuffer_, *indexBuffer_;
		int vertexCount_, indexCount_;

		TextureClass* texture_;

		// Cal3D Model
		CalCoreModel* cal3dCoreModel_;
		CalModel* cal3dModel_;
		// Meshes
		int modelMeshID;
		// Animations
		std::map<std::string, int> animations_;
		std::string animationToPlay_;
		std::string initialAnimationToPlay_;
};

#endif
