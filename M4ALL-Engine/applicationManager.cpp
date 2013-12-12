#include "applicationManager.h"

#include "applicationState.h"
#include "introScreenState.h"

ApplicationManager::ApplicationManager()
{
	appState_ = 0;
	graphicsManager_ = 0;
}

ApplicationManager::~ApplicationManager()
{

}

bool ApplicationManager::setup(HWND windowsHandler, int width, int height, bool fullscreen)
{
	windowHandler_ = windowsHandler;

	graphicsManager_ = new GraphicsManager;
	if(!graphicsManager_)
	{
		return false;
	}

	if(!graphicsManager_->setup(width, height, false, windowsHandler, fullscreen, 1000.0f, 0.1f))
	{
		MessageBox(NULL, L"Could not initialize DirectX11", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	if(!changeState(IntroScreenState::Instance()))
	{
		return false;
	}

	return true;
}

void ApplicationManager::update()
{

	// we update the State with the frame elapsed time
	appState_->update(this, 0);
}

void ApplicationManager::draw()
{
	graphicsManager_->beginDraw(1.0f, 0.5f, 0.0f, 1.0f);

	// We call the draw function of the active state
	appState_->draw(this);

	graphicsManager_->endDraw();
}

void ApplicationManager::destroy()
{
	appState_->destroy();

	if(graphicsManager_)
	{
		graphicsManager_->destroy();
		delete graphicsManager_;
		graphicsManager_ = 0;
	}
}

bool ApplicationManager::changeState(ApplicationState* appState)
{
	if (appState != appState_) {
		appState_ = appState;
		return appState_->setup(this, windowHandler_);
	}

	return true;
}
