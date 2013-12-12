#include "applicationManager.h"

#include "applicationState.h"
#include "introScreenState.h"

ApplicationManager::ApplicationManager()
{
	appState_ = 0;
}

ApplicationManager::~ApplicationManager()
{

}

bool ApplicationManager::setup(HWND windowsHandler, int width, int height, bool fullscreen)
{
	windowHandler_ = windowsHandler;

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
	// We call the draw function of the active state
	appState_->draw(this);
}

void ApplicationManager::destroy()
{
	appState_->destroy();
}

bool ApplicationManager::changeState(ApplicationState* appState)
{
	if (appState != appState_) {
		appState_ = appState;
		return appState_->setup(this, windowHandler_);
	}

	return true;
}
