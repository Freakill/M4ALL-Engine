#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <windows.h>

#include <string>
#include <sstream>

#include "notifierClass.h"

class InputManager : public Notifier<InputManager, int>
{
public:
	InputManager();
	InputManager(const InputManager&);
	~InputManager();

	void setup();

	void keyDown(unsigned int input);
	void keyUp(unsigned int input);

private:
	bool keys_[256];
};

#endif