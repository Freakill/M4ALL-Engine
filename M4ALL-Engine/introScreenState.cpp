#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "introScreenState.h"

#include "firstScreenState.h"

IntroScreenState IntroScreenState::introScreenState_;

IntroScreenState::IntroScreenState()
{
	camera_ = 0;
	background_ = 0;
}

IntroScreenState::~IntroScreenState()
{

}

IntroScreenState* IntroScreenState::Instance()
{
	return (&introScreenState_);
}

bool IntroScreenState::setup(ApplicationManager* appManager, GraphicsManager* graphicsManager, InputManager * inputManager, HWND windowHandler)
{
	appManager_ = appManager;

	// We get a pointer to the graphicsManager
	graphicsManager_ = graphicsManager;

	D3DXMATRIX baseViewMatrix;

	// Create the camera object.
	camera_ = new CameraClass;
	if(!camera_)
	{
		return false;
	}

	// Set the initial position of the camera.
	camera_->setPosition(0.0f, 2.5f, -10.0f);
	camera_->draw();
	camera_->getViewMatrix(baseViewMatrix);

	// Create the bitmap object.
	background_ = new ImageClass();
	if(!background_)
	{
		return false;
	}

	RECT dimensions; //Rectangle of the active region of the screen window
    GetClientRect(windowHandler, &dimensions);

    int screenWidth = dimensions.right - dimensions.left;
    int screenHeight = dimensions.bottom - dimensions.top;

	// Initialize the bitmap object.
	if(!background_->setup(graphicsManager_->getDevice(), screenWidth, screenHeight, "../M4ALL-Engine/data/seafloor.dds", screenWidth, screenHeight))
	{
		MessageBoxA(NULL, "Could not initialize the image object.", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

	inputManager->AddListener(*this);

	return true;
}

void IntroScreenState::update(float elapsedTime)
{	
	
}

void IntroScreenState::draw()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix, scalingMatrix, zMatrix, xMatrix;

	// Generate the view matrix based on the camera's position.
	camera_->draw();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera_->getViewMatrix(viewMatrix);
	graphicsManager_->getWorldMatrix(worldMatrix);
	graphicsManager_->getProjectionMatrix(projectionMatrix);
	graphicsManager_->getOrthoMatrix(orthoMatrix);

	graphicsManager_->turnZBufferOff();

	//Put the image vertex and index buffers on the graphics pipeline to prepare them for drawing
	background_->draw(graphicsManager_->getDeviceContext(), 0, -2);

	graphicsManager_->draw2D(background_->getIndexCount(), worldMatrix, viewMatrix, orthoMatrix, background_->getTexture(), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f));

	graphicsManager_->turnZBufferOn();
}

void IntroScreenState::destroy()
{
	if(background_)
	{
		background_->destroy();
		delete background_;
		background_ = 0;
	}

	// Release the camera object.
	if(camera_)
	{
		delete camera_;
		camera_ = 0;
	}
}

void IntroScreenState::Notify(InputManager* notifier, int arg)
{
	switch(arg){
		case 13:
			{
				changeState(FirstScreenState::Instance());
			}
			break;
		default:
			break;
	}
}
