#ifndef _APPLICATION_STATE_H_
#define _APPLICATION_STATE_H_

#include "applicationManager.h"

#include <string>

class ApplicationState
{
	public:
		ApplicationState();
		virtual ~ApplicationState();

		virtual bool setup(ApplicationManager* appManager, HWND windowHandler) = 0;
		virtual void update(ApplicationManager* appManager, float elapsedTime) = 0;
		virtual void draw(ApplicationManager* appManager) = 0;
		virtual void destroy() = 0;

		void changeState(ApplicationManager* appManager, ApplicationState* appState);
};

#endif
