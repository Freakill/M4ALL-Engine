#include "applicationState.h"

#include "applicationManager.h"

ApplicationState::ApplicationState()
{
	appManager_ = 0;
}

ApplicationState::~ApplicationState()
{

}

void ApplicationState::changeState(ApplicationState* appState)
{
	if(appManager_)
	{
		appManager_->changeState(appState);
	}
}


