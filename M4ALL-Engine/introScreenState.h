#ifndef _INTRO_SCREEN_STATE_H_
#define _INTRO_SCREEN_STATE_H_

#include "applicationState.h"

#include <mmsystem.h>

class IntroScreenState: public ApplicationState
{
	public:
		IntroScreenState();
		virtual ~IntroScreenState();
		static IntroScreenState* Instance();

		virtual bool setup(ApplicationManager* appManager, HWND windowHandler);
		virtual void update(ApplicationManager* appManager, float elapsedTime);
		virtual void draw(ApplicationManager* appManager);
		virtual void destroy();

	private:
		static IntroScreenState introScreenState_; //singleton
};

#endif //_INTRO_SCREEN_STATE_H_
