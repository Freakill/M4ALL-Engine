#include "applicationState.h"

#include "applicationManager.h"

ApplicationState::ApplicationState()
{

}

ApplicationState::~ApplicationState()
{

}

void ApplicationState::changeState(ApplicationManager* appManager, ApplicationState* appState)
{
	appManager->changeState(appState);
}


