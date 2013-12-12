#include "inputManager.h"

InputManager::InputManager()
{

}

InputManager::InputManager(const InputManager& other)
{

}

InputManager::~InputManager()
{

}

void InputManager::setup()
{
	// Initialize all the keys to being released and not pressed.
	for(int i=0; i<256; i++)
	{
		keys_[i] = false;
	}

	return;
}

void InputManager::keyDown(unsigned int input)
{
	if(!keys_[input]){
		NotifyListeners((int)input);
	}

	// If a key is pressed then save that state in the key array.
	keys_[input] = true;
	return;
}

void InputManager::keyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	keys_[input] = false;
	return;
}
