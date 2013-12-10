#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

#include <windows.h>

#include <string>
#include <sstream>

class InputManager
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