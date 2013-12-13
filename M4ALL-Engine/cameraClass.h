#ifndef _CAMERA_CLASS_H_
#define _CAMERA_CLASS_H_

#include <d3dx10math.h>

class CameraClass
{
	public:
		CameraClass();
		CameraClass(const CameraClass&);
		~CameraClass();

		void setPosition(float x, float y, float z);
		void setRotation(float x, float y, float z);

		D3DXVECTOR3 getPosition();
		D3DXVECTOR3 getRotation();

		void draw();
		void getViewMatrix(D3DXMATRIX& viewMatrix);

	private:
		float positionX_, positionY_, positionZ_;
		float rotationX_, rotationY_, rotationZ_;
		D3DXMATRIX viewMatrix_;
};

#endif