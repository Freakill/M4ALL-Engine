#ifndef _SYSTEM_CLASS_H_
#define _SYSTEM_CLASS_H_

// Define to speed up building process, by reducing Win32 header files
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "inputManager.h"
#include "listenerClass.h"
#include "applicationManager.h"

class SystemClass : public Listener<InputManager, int>
{
	public:
		SystemClass();
		SystemClass(const SystemClass&);
		~SystemClass();
				
		bool setup();
		bool setup(int width, int height);
		void run();
		void destroy();
		void processEvents();

		void swapBuffers();

		// Methods for handling system messages, the static re-routes the messages to the WindowProcess
		static LRESULT CALLBACK StaticWindowProcess(HWND windowHandler, UINT messageCode, WPARAM wAdditionalData, LPARAM lAdditionalData);
		LRESULT CALLBACK WindowProcess(HWND windowHandler, UINT messageCode, WPARAM wAdditionalData, LPARAM lAdditionalData);

	private:
		void setupPixelFormat(void);
		
		virtual void Notify(InputManager* notifier, int arg){
			switch(arg){
				case 27:
				{
					if(MessageBox(windowHandler_, (LPCWSTR)L"Really quit?", (LPCWSTR)L"My application", MB_OKCANCEL) == IDOK)
						isRunning_ = false;
				}
				break;
				default:
				{
					std::stringstream keyStream;
					keyStream << "Key pressed " << arg << " has no behaviour attached";
					MessageBoxA(NULL, keyStream.str().c_str(), "SystemClass", MB_OK);
				}
				break;
			}
		}

		bool isRunning_;
		bool fullscreen_;

		LPCWSTR applicationName_;
		HWND windowHandler_;
		HDC deviceContextHandler_;
		HGLRC renderingContextHandler_;
		HINSTANCE instanceHandler_;
		WNDCLASSEX windowClass_; //Contains window class information. (http://msdn.microsoft.com/en-us/library/windows/desktop/ms633577%28v=vs.85%29.aspx)
		RECT windowRectangle_; //Windows size
		DWORD windowExtendedStyle_; //Window Extended Style
		DWORD windowStyle_; //Window Style
		DEVMODE deviceModeSettings_;

		InputManager* inputManager_;
		ApplicationManager* appManager_;
};

#endif //_SYSTEM_CLASS_H_
