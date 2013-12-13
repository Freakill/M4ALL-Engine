#ifndef _INTRO_SCREEN_STATE_H_
#define _INTRO_SCREEN_STATE_H_

#include "applicationState.h"

#include <mmsystem.h>

class IntroScreenState : public ApplicationState
{
	public:
		IntroScreenState();
		virtual ~IntroScreenState();
		static IntroScreenState* Instance();

		virtual bool setup(ApplicationManager* appManager, GraphicsManager* graphicsManager, InputManager * inputManager, HWND windowHandler);
		virtual void update(float elapsedTime);
		virtual void draw();
		virtual void destroy();

	private:
		static IntroScreenState introScreenState_; //singleton

		virtual void Notify(InputManager* notifier, int arg);

		CameraClass*	camera_;
		ImageClass*		background_;
};

#endif //_INTRO_SCREEN_STATE_H_
