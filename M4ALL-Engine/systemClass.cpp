#include "systemClass.h"

SystemClass::SystemClass()
{
	// Initializing objects at 0 for control
	inputManager_ = 0;
	appManager_ = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::setup()
{
	return setup(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

bool SystemClass::setup(int width, int height)
{
	applicationName_ = L"M4ALL";

	windowRectangle_.left			= (long) (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
	windowRectangle_.right			= (long) width;
	windowRectangle_.top			= (long) (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	windowRectangle_.bottom			= (long) height;

	windowClass_.cbSize				= sizeof(WNDCLASSEX);				// The size, in bytes, of this structure.
	windowClass_.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass_.lpfnWndProc		= SystemClass::StaticWindowProcess;	// A pointer to an application-defined function called the window procedure or "window proc." 
	windowClass_.cbClsExtra			= 0;
    windowClass_.cbWndExtra			= 0;
    windowClass_.hInstance			= instanceHandler_;					// A handle to the instance that contains the window procedure for the class. 
    windowClass_.lpszClassName		= applicationName_;					// A pointer to a null-terminated string or is an atom.
	windowClass_.hCursor			= LoadCursor(NULL, IDC_ARROW);		// A handle to the class cursor.
	windowClass_.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);		// A handle to a small icon that is associated with the window class.
	windowClass_.hIcon				= LoadIcon(NULL, IDI_APPLICATION);  // Default icon
    windowClass_.hbrBackground		= (HBRUSH)GetStockObject(BLACK_BRUSH);// Don't need background
    windowClass_.lpszMenuName		= NULL; 

    if(!RegisterClassEx(&windowClass_))// Registers a window class with the operating system for subsequent use in calls to the CreateWindow or CreateWindowEx function.
	{
		return false;
	}

	// Ask at the beginning of the program whether the user wants the application to work in full screen or not. For debug purposes we leave it always in false
	/*if(MessageBox(NULL, L"Do you want to launch application at full screen?", L"Full Screen", MB_ICONINFORMATION | MB_OKCANCEL) != IDCANCEL)
	{
		fullscreen_ = true;
	}
	else
	{
		fullscreen_ = false;
	}*/

	fullscreen_ = false;	

	if(fullscreen_)
	{
		memset(&deviceModeSettings_, 0, sizeof(deviceModeSettings_));
		deviceModeSettings_.dmSize = sizeof(deviceModeSettings_);

		deviceModeSettings_.dmPelsWidth	    = width;	// Setting device mode width to specified width
		deviceModeSettings_.dmPelsHeight	= height;	// Setting device mode height to specified height
		deviceModeSettings_.dmBitsPerPel	= 16;		// Setting device bits per pixel
		deviceModeSettings_.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if(ChangeDisplaySettings(&deviceModeSettings_, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) { 
			// Setting device mode for full screen has failed, switch to windowed
			MessageBox(NULL, L"Display mode failed", L"Error", MB_ICONERROR | MB_OK);
			fullscreen_ = false;
		}
		// ChangeDisplaySettings(0, 0) -> for back to normal windowed screen
	}
	else
	{
		fullscreen_ = false;
	}

	if(fullscreen_)
	{
		windowExtendedStyle_ = WS_EX_APPWINDOW;
		windowStyle_ = WS_POPUP;
		ShowCursor(false);
	}
	else 
	{
		windowExtendedStyle_ = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		//windowStyle_ = WS_OVERLAPPEDWINDOW; //NORMAL STYLE
		windowStyle_ = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP; //NO BORDER STYLE
	}

	if(!AdjustWindowRectEx(&windowRectangle_, windowStyle_, false, windowExtendedStyle_))
	{
		return false;
	}
    
    windowHandler_ = CreateWindowEx(							// Creates an overlapped, pop-up, or child window with an extended window style.
        NULL,													// Optional window styles.
        applicationName_,										// Window class
        L"Learn to Program Windows",							// Window text
		windowStyle_,											// Window style
        windowRectangle_.left, windowRectangle_.top,			// Size and position
		width, height,
        NULL,													// Parent window    
        NULL,													// Menu
        instanceHandler_,										// Instance handle
        this													// Additional application data
        );

    if(windowHandler_ == NULL)
	{
		return false;
	}

	ShowWindow(windowHandler_, SW_SHOW);
	UpdateWindow(windowHandler_); // update the window
	SetFocus(windowHandler_);

	inputManager_ = new InputManager;
	if(!inputManager_)
	{
		return false;
	}
	inputManager_->setup();

	// Initialize the Application Manager
	appManager_ = new ApplicationManager;
	if(!appManager_)
	{
		return false;
	}
	if(!appManager_->setup(windowHandler_, width, height, fullscreen_))
	{
		return false;
	}

	// Initialize the listeners for input
	inputManager_->AddListener(*this);

	return true;
}

void SystemClass::run()
{
	while(isRunning_)
	{
		processEvents(); //Process any window events

		appManager_->update();
		appManager_->draw();

		swapBuffers();
	}

	exit(0);

	return;

}

void SystemClass::destroy()
{
	if(inputManager_)
	{
		delete inputManager_;
		inputManager_ = 0;
	}

	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(fullscreen_)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(windowHandler_);
	windowHandler_ = NULL;

	// Remove the application instance.
	UnregisterClass(applicationName_, instanceHandler_);
	instanceHandler_ = NULL;
}

void SystemClass::processEvents()
{
    MSG msg;

    //While there are messages in the queue, store them in msg
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        //Process the messages one-by-one
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void SystemClass::setupPixelFormat(void)
{
    int pixelFormat;

    PIXELFORMATDESCRIPTOR pfd =
    {   
        sizeof(PIXELFORMATDESCRIPTOR),  // size
            1,                          // version
            PFD_SUPPORT_OPENGL |        // OpenGL window
            PFD_DRAW_TO_WINDOW |        // render to window
            PFD_DOUBLEBUFFER,           // support double-buffering
            PFD_TYPE_RGBA,              // color type
            32,                         // prefered color depth
            0, 0, 0, 0, 0, 0,           // color bits (ignored)
            0,                          // no alpha buffer
            0,                          // alpha bits (ignored)
            0,                          // no accumulation buffer
            0, 0, 0, 0,                 // accum bits (ignored)
            16,                         // depth buffer
            0,                          // no stencil buffer
            0,                          // no auxiliary buffers
            PFD_MAIN_PLANE,             // main layer
            0,                          // reserved
            0, 0, 0,                    // no layer, visible, damage masks
    };

    pixelFormat = ChoosePixelFormat(deviceContextHandler_, &pfd);
    SetPixelFormat(deviceContextHandler_, pixelFormat, &pfd);
}

void SystemClass::swapBuffers()
{ 
	SwapBuffers(deviceContextHandler_); 
}

LRESULT SystemClass::WindowProcess(HWND windowHandler, UINT messageCode, WPARAM wAdditionalData, LPARAM lAdditionalData)
{
    switch (messageCode)
	{
		case WM_CREATE:
		{
			deviceContextHandler_ = GetDC(windowHandler);
			setupPixelFormat();

// Here we would initialize the OpenGL context calling to graphics manager from application manager
#if defined(OPENGL)

#endif

			isRunning_ = true; //Mark our window as running
		}
		break;
		case WM_CLOSE:
		{
			//if (MessageBox(windowHandler, (LPCWSTR)L"Really quit?", (LPCWSTR)L"My application", MB_OKCANCEL) == IDOK)
			DestroyWindow(windowHandler);
			//else
			//	return 0;
		}
		break;
		case WM_DESTROY:
		{
// Here we would destroy the OpenGL context calling to graphics manager from application manager
#if defined(OPENGL)

#endif
			isRunning_ = false;

			PostQuitMessage(0);
			return 0;
		}
		break;
		/*case WM_SIZE:
		{
			int width = LOWORD(lAdditionalData);  // Macro to get the low-order word.
			int height = HIWORD(lAdditionalData); // Macro to get the high-order word.

			// Respond to the message:
			onResize(width, height);
		}
		break;*/
		case WM_KEYDOWN:
		{
			inputManager_->keyDown((unsigned int)wAdditionalData);
			return 0;
		}
		break;
		case WM_KEYUP:
		{
			inputManager_->keyUp((unsigned int)wAdditionalData);
			return 0;
		}
		break;
		default:
			break;
    }

    return DefWindowProc(windowHandler, messageCode, wAdditionalData, lAdditionalData);
}

LRESULT SystemClass::StaticWindowProcess(HWND windowHandler, UINT messageCode, WPARAM wAdditionalData, LPARAM lAdditionalData) 
{
	SystemClass* window = NULL;

    //If this is the create message
    switch(messageCode)
	{
		case WM_CREATE:
		{
			//Get the pointer we stored during create
			window = (SystemClass*)((LPCREATESTRUCT)lAdditionalData)->lpCreateParams;

			//Associate the window pointer with the hwnd for the other events to access
			SetWindowLongPtr(windowHandler, GWL_USERDATA, (LONG_PTR)window);
		}
		break;
		default:
		{
			//If this is not a creation event, then we should have stored a pointer to the window
			window = (SystemClass*)GetWindowLongPtr(windowHandler, GWL_USERDATA);

			if(!window) 
				return DefWindowProc(windowHandler, messageCode, wAdditionalData, lAdditionalData);    
		}
		break;
	}

    //Call our window's member WndProc (allows us to access member variables)
	return window->WindowProcess(windowHandler, messageCode, wAdditionalData, lAdditionalData);
}
