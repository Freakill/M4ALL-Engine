#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "introScreenState.h"

IntroScreenState IntroScreenState::introScreenState_;

IntroScreenState::IntroScreenState()
{
	
}

IntroScreenState::~IntroScreenState()
{

}

IntroScreenState* IntroScreenState::Instance()
{
	return (&introScreenState_);
}

bool IntroScreenState::setup(ApplicationManager* appManager, HWND windowHandler)
{

	return true;
}

void IntroScreenState::update(ApplicationManager* appManager, float elapsedTime)
{	
	
}

void IntroScreenState::draw(ApplicationManager* appManager)
{

}

void IntroScreenState::destroy()
{
	
}
