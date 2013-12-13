////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraClass.h"


CameraClass::CameraClass()
{
	positionX_ = 0.0f;
	positionY_ = 0.0f;
	positionZ_ = 0.0f;

	rotationX_ = 0.0f;
	rotationY_ = 0.0f;
	rotationZ_ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::setPosition(float x, float y, float z)
{
	positionX_ = x;
	positionY_ = y;
	positionZ_ = z;
	return;
}


void CameraClass::setRotation(float x, float y, float z)
{
	rotationX_ = x;
	rotationY_ = y;
	rotationZ_ = z;
	return;
}


D3DXVECTOR3 CameraClass::getPosition()
{
	return D3DXVECTOR3(positionX_, positionY_, positionZ_);
}


D3DXVECTOR3 CameraClass::getRotation()
{
	return D3DXVECTOR3(rotationX_, rotationY_, rotationZ_);
}


void CameraClass::draw()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotation_matrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = positionX_;
	position.y = positionY_;
	position.z = positionZ_;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotationX_ * 0.0174532925f;
	yaw   = rotationY_ * 0.0174532925f;
	roll  = rotationZ_ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotation_matrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotation_matrix);
	D3DXVec3TransformCoord(&up, &up, &rotation_matrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix_, &position, &lookAt, &up);

	return;
}


void CameraClass::getViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = viewMatrix_;
	return;
}