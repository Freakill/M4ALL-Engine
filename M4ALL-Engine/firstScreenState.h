#ifndef _FIRST_SCREEN_STATE_H_
#define _FIRST_SCREEN_STATE_H_

#include "applicationState.h"

#include <mmsystem.h>

class FirstScreenState : public ApplicationState
{
	public:
		FirstScreenState();
		virtual ~FirstScreenState();
		static FirstScreenState* Instance();

		virtual bool setup(ApplicationManager* appManager, GraphicsManager* graphicsManager, InputManager * inputManager, HWND windowHandler);
		virtual void update(float elapsedTime);
		virtual void draw();
		virtual void destroy();

	private:
		static FirstScreenState firstScreenState_; //singleton

		virtual void Notify(InputManager* notifier, int arg);

		CameraClass*	camera_;
		ImageClass*		background_;
};

#endif //_FIRST_SCREEN_STATE_H_
