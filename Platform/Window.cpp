/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _LINUX
	#include <X11/Xatom.h>
#endif // _LINUX
#include "Platform/Application.h"
#include "Platform/Input/InputManager.h"
#include "Math/MathHelper.h"
#include "Platform/Window.h"
#include "Platform/FailureHandling/WindowException.h"

using namespace FailureHandling;
using namespace Platform;
using namespace Input;
using namespace std;
using namespace Utilities;

#ifdef _WINDOWS
	const char *Window::CLASS_NAME = "BaseProjectWindow";
#endif // _WINDOWS

Platform::Window::Window
	(
		#ifdef _WINDOWS
			HINSTANCE applicationHandle,
		#endif // _WINDOWS
		const string &title, bool fullScreen, const ImgSize &size,
		uint16 depthBitsPerPixel, uint16 stencilBitsPerPixel
	) :
	#ifdef _WINDOWS
		mApplicationHandle(applicationHandle),
		mWindowHandle(NULL),
		mScreenSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)),
	#elif _LINUX
		mWindowHandle(0), mDisplay(NULL),
	#endif // _WINDOWS
	mTitle(title), mSize(size),
	mDepthBitsPerPixel(depthBitsPerPixel),
	mStencilBitsPerPixel(stencilBitsPerPixel),
	mActive(false), mFullScreen(fullScreen)
{
	bool createdWindow = false;

	#ifdef _WINDOWS
		registerWindowType(applicationHandle);
		createdWindow = createWindow(applicationHandle);
		
	#elif _LINUX
		/// get active display
		mDisplay = XOpenDisplay(NULL);
		if (NULL == mDisplay)
			throw WindowException("Cannot connect to X server.");

		// how many display devices?
		int32 screenCount = XScreenCount(mDisplay);
		if (screenCount < 1)
			throw WindowException("There is no screen available for display.");

		// set up frame, depth & stencil buffers
		int32 defaultScreenID	= XDefaultScreen(mDisplay);
		XVisualInfo	visualInfo;
		Status success = XMatchVisualInfo(mDisplay, defaultScreenID, mDepthBitsPerPixel, TrueColor, &visualInfo);
		if (!success)
			throw WindowException("Could not choose a proper visual for the main window to be created.");

		createdWindow = createWindow(visualInfo);
		
	#else
		assert(false);
		
	#endif // _WINDOWS

	if (!createdWindow)
		throw WindowException("Could not create a window.");
}

#ifdef _WINDOWS
	bool Platform::Window::createWindow(HINSTANCE applicationHandle)
	{
		// Create a window either in fullScreen mode or , in release mode, produce a window without border and caption in the middle of the display screen.
		// The window is also displayed and updated.
		DWORD style = WS_POPUP | WS_VISIBLE;

		if (mFullScreen)
		{
			style |= WS_MAXIMIZE;

			mWindowHandle = CreateWindowExA(0, CLASS_NAME, mTitle.c_str(), style, 0, 0, mScreenSize[0], mScreenSize[1], 0, 0, applicationHandle, 0);
			if (0 == mWindowHandle)
				return false;

			ShowWindow(mWindowHandle, SW_SHOWMAXIMIZED);
		}
		else
		{
			int32 xPos = static_cast<int>((mScreenSize[0] - mSize[0] + 0.5f) * 0.5f);
			int32 yPos = static_cast<int>((mScreenSize[1] - mSize[1] + 0.5f) * 0.5f);
			style = WS_CAPTION  | WS_SYSMENU | WS_MINIMIZEBOX;

			mWindowHandle = CreateWindowExA(0, CLASS_NAME, mTitle.c_str(), style, xPos, yPos, mSize[0], mSize[1], 0, 0, applicationHandle, 0);
			if (0 == mWindowHandle)
				return false;

			ShowWindow(mWindowHandle, SW_SHOW);
		}

		UpdateWindow(mWindowHandle);
		ShowCursor(!mFullScreen);
		return true;
	}

	void Window::registerWindowType(HINSTANCE applicationHandle) const
	{
		// create a window description connected to the function windowProcedure
		// default or standard values are used
		// register this description for later use in createWindow
		WNDCLASSEXA windowClassDescription;
		windowClassDescription.cbSize = sizeof(WNDCLASSEXA);
		windowClassDescription.style = 0;
		windowClassDescription.lpfnWndProc = applicationProcedure;
		windowClassDescription.cbClsExtra = 0;
		windowClassDescription.cbWndExtra = 0;
		windowClassDescription.hInstance = applicationHandle;
		windowClassDescription.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		windowClassDescription.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClassDescription.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
		windowClassDescription.lpszMenuName = 0;
		windowClassDescription.lpszClassName = CLASS_NAME;
		windowClassDescription.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassExA(&windowClassDescription))
			throw WindowException("Could not register the required game window description with RegisterClass.");
	}

#elif _LINUX
	bool Platform::Window::createWindow(XVisualInfo &visualInfo)
	{
		// get root window
		::Window root = DefaultRootWindow(mDisplay);

		// set XWindow attributes and create XWindow
		Colormap colorMap = XCreateColormap(mDisplay, root, visualInfo.visual, AllocNone);

		XSetWindowAttributes attributes;
		attributes.colormap				= colorMap;
		attributes.event_mask			= KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
			PointerMotionMask | ExposureMask | VisibilityChangeMask | FocusChangeMask;
		attributes.background_pixmap	= None;
		attributes.background_pixel		= 0;
		attributes.border_pixel			= 0;
		unsigned long setAttributes		= CWBackPixmap | CWBorderPixel | CWColormap | CWEventMask;

//		// determine width, height and the position of the upper left corner of the window
//		if (mFullScreen)
//		{
//			Screen	*screen = DefaultScreenOfDisplay(mDisplay);
//					mSize[0]	= screen->width;
//					mSize[1]	= screen->height;
//		}

		mWindowHandle = XCreateWindow(mDisplay, root, 0, 0, mSize[0], mSize[1], 0,
				visualInfo.depth, InputOutput, visualInfo.visual, setAttributes, &attributes);
		if (0 == mWindowHandle)
			return false;

		// set window name and show the created window
		//const char *chars = mTitle.c_str();
		char *list[] = { (char *) mTitle.c_str() };
		XTextProperty textProperty;
		XmbTextListToTextProperty(mDisplay, list, 1, XStringStyle, &textProperty);

		XSetWMName(mDisplay, mWindowHandle, &textProperty);
		XMapWindow(mDisplay, mWindowHandle);

		// inform X window system that window is supposed to be viewed on the complete screen
		if (mFullScreen)
		{
			// change full screen window property
			Atom	propertiesType	= XInternAtom(mDisplay, "_NET_WM_STATE", False);
			Atom	properties[2]	= { XInternAtom(mDisplay, "_NET_WM_STATE_FULLSCREEN", False), None };
			uint32	propertyCount	= 1;
			uint32	atomBitSizes	= 32;
			XChangeProperty(mDisplay, mWindowHandle, propertiesType, XA_ATOM, atomBitSizes,
							PropModeReplace, (uint8 *) properties, propertyCount);
		}

//		// get actual window size as there might be a virtual window (= xlib screen) for multiple devices while this window is only shown on one device
//		// -> xlib screen size != full screen window resolution
//		XWindowAttributes windowAttributes;
//		XGetWindowAttributes(mDisplay, mWindowHandle, &windowAttributes);
//		mSize[0]	= windowAttributes.width;
//		mSize[1]	= windowAttributes.height;

		return true;
	}

#endif // _WINDOWS

Platform::Window::~Window()
{
	#ifdef _WINDOWS
		DestroyWindow(mWindowHandle);
		UnregisterClassA(CLASS_NAME, mApplicationHandle);

	#else
		XDestroyWindow(mDisplay, mWindowHandle);
		XCloseDisplay(mDisplay);

	#endif // WINDOWS
}

#ifdef _WINDOWS
	// this function is called by windows and it calls the window procedure of the main window
	// it just delegates messages
	LRESULT CALLBACK Window::applicationProcedure(HWND windowHandle, uint32 messageIdentifier, WPARAM wParam, LPARAM lParam)
	{
		if (Window::exists())
			return Window::getSingleton().windowProcedure(windowHandle, messageIdentifier, wParam, lParam);
		else
			return DefWindowProcA(windowHandle, messageIdentifier, wParam, lParam);
	}

	LRESULT CALLBACK Window::windowProcedure(HWND windowHandle, uint32 messageIdentifier, WPARAM wParam, LPARAM lParam)
	{
		switch(messageIdentifier)
		{
			case WM_ACTIVATE:
			{
				if (WA_INACTIVE == LOWORD(wParam))
					onActivityChange(false);
				else
					onActivityChange(true);
				return 0;
			}

			case WM_CHAR:	// interpret text input
			{
				if (!Application::getSingleton().isInterpretingTextInput())
					break;
				Application::getSingleton().getTextInput().processWideCharacterInput(static_cast<wchar_t>(wParam));
				return 0;
			}

//			case WM_ENTERSIZEMOVE:	// start of window resizing
//				return 0;
//
//			case WM_EXITSIZEMOVE:	// end of window resizing
//			{
//				for (uint32 i = 0; i < mObservers.size(); ++i)
//					mObservers[i]->onResize();
//				return 0;
//			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}

//			case WM_SIZE:
//			{
//				mSize[0] = LOWORD(lParam);
//				mSize[1] = HIWORD(lParam);
//
//				for (uint32 i = 0; i < mObservers.size(); ++i)
//					mObservers[i]->onResize();	// should not be here!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//				switch(wParam)
//				{
//				case SIZE_MINIMIZED:
//					break;
//
//				case SIZE_MAXIMIZED:
//					break;
//
//				case SIZE_RESTORED:
//					break;
//
//				default:
//					break;
//				}
//				return 0;
//			}

			default:
			{
				break;
			}
		}

		return DefWindowProcA(windowHandle, messageIdentifier, wParam, lParam);
	}

#elif _LINUX
	bool Platform::Window::processQueuedEvent()
	{
		// https://tronche.com/gui/x/xlib/events/keyboard-pointer/keyboard-pointer.html#XButtonEvent
		int32 eventsCount =  XQLength(mDisplay);
		if (0 == eventsCount)
			eventsCount = XPending(mDisplay);
		if (0 == eventsCount)
			return false;

		// get and process event
		XEvent event;
		XNextEvent(mDisplay, &event);

		switch(event.type)
		{
//			case ResizeRequest:
//				glViewport(0, 0, mSize[0], mSize[1]);
//				break;

			case MappingNotify:
			{
				// some kind of mapping changed: pointer, keyboard or modifier mapping
				XMappingEvent &mappingEvent = event.xmapping;

				switch (mappingEvent.request)
				{
					case MappingModifier:
					case MappingKeyboard:
						XRefreshKeyboardMapping(&mappingEvent);
						break;

					case MappingPointer:
						break;

					default:
						assert(false);
						break;
				}
			}

			case Expose:
				onActivityChange(true);
				break;

			case FocusIn: // got input focus
				onActivityChange(true);
				break;

			case UnmapNotify: // window was unmapped
				onActivityChange(false);
				break;

			case FocusOut: // lost input focus
				onActivityChange(false);
				break;

			case DestroyNotify:
				onActivityChange(false);
				break;

			// window is not, partially or completely visible?
			case VisibilityNotify:
			{
				if (VisibilityUnobscured == event.xvisibility.state)
				{
					onActivityChange(true);
				}
				else if (VisibilityFullyObscured == event.xvisibility.state ||
						 VisibilityPartiallyObscured == event.xvisibility.state)
				{
					onActivityChange(false);
				}

				break;
			}

            case KeyPress:
			case KeyRelease:
            {
				const uint32	BUFFER_LENGTH	= 10;
				bool			pressed			= (KeyPress == event.type);
				char			string[BUFFER_LENGTH];
				KeySym			keySym;

				// get key translation
				int len = XLookupString(&event.xkey, string, BUFFER_LENGTH, &keySym, NULL);
				if (len > 0  && pressed)
				{
					// todo: actually there can be strings instead of single characters
					Application &app = Application::getSingleton();
					if (app.isInterpretingTextInput())
                        app.getTextInput().processWideCharacterInput(string[0]);
				}

				// translate Linux KeySym to this projects & platform independent Key value
                Key key = keySymToKey(keySym);

				Keyboard &keyboard = (Keyboard &) InputManager::getSingleton().getKeyboard();
				keyboard.processKeyEvent(key, pressed);
                break;
            }

            case ButtonPress:
			case ButtonRelease:
            {
				bool			pressed	= (ButtonPress == event.type);
				Mouse			&mouse	= (Mouse &) InputManager::getSingleton().getMouse();
				Mouse::Button	button	= Mouse::BUTTON_FAIL;

				switch (event.xbutton.button)
				{
					// is it an actual mouse button event?
					case 1:
						button = Mouse::BUTTON_PRIMARY;
						mouse.processButtonEvent(button, pressed);
						break;

					case 3:
						button = Mouse::BUTTON_SECONDARY;
						mouse.processButtonEvent(button, pressed);
						break;

					case 6:
						button = Mouse::BUTTON_MIDDLE;
						mouse.processButtonEvent(button, pressed);
						break;

					case 8:
						button = Mouse::BUTTON_BACK;
						mouse.processButtonEvent(button, pressed);
						break;

					case 9:
						button = Mouse::BUTTON_FORWARD;
						mouse.processButtonEvent(button, pressed);
						break;

					// is it a scroll wheel event?
					// xlib is somewhat odd: scroll wheel events are considered to be mouse button 4 and 5 events
					case 4:
						mouse.processWheelMotionEvent(true);
						break;

					case 5:
						mouse.processWheelMotionEvent(false);
						break;

					default:
					{
						// todo log this;
						break;
					}
				}

				break;
            }

			case MotionNotify:
			{
				Mouse &mouse = (Mouse &) InputManager::getSingleton().getMouse();

				// get pointer coordinates relative to event window
				float x =   2.0f * (Real) event.xmotion.x / mSize[0] - 1.0f;
				float y = -(2.0f * (Real) event.xmotion.y / mSize[1] - 1.0f);

				// is necessary due to possible window movements
				Math::clamp(x, 1.0f, -1.0f);
				Math::clamp(y, 1.0f, -1.0f);

				mouse.processMotionEvent(x, y);
				break;
            }

			default:
				int test = event.type;
				assert(false);
				break;
		}

		return eventsCount > 1;
	}

	void Platform::Window::setVisual(XVisualInfo &visualInfo)
	{
		XDestroyWindow(mDisplay, mWindowHandle);
		if (!createWindow(visualInfo))
			throw WindowException("Could change current visual and recreate a window based on the new visual.");
	}

    Key Platform::Window::keySymToKey(KeySym keySym)
	{
		Key key;
		switch (keySym)
		{
			case XK_Sys_Req:        return KEY_SYSRQ;
			case XK_Print:			return KEY_SYSRQ;
			case XK_Scroll_Lock:	return KEY_SCROLL;
			case XK_BackSpace:		return KEY_BACKSPACE;
			case XK_Return:         return KEY_RETURN;
			case XK_Prior:          return KEY_PGUP;
			case XK_Next:           return KEY_PGDN;
			case XK_Tab:			return KEY_TAB;
			case XK_Pause:			return KEY_PAUSE;
			case XK_Escape:			return KEY_ESCAPE;
			case XK_Delete:			return KEY_DELETE;
			case XK_Home:			return KEY_HOME;
			case XK_Left:			return KEY_LEFT;
			case XK_Up:				return KEY_UP;
			case XK_Right:			return KEY_RIGHT;
			case XK_Down:			return KEY_DOWN;

			case XK_A:				return KEY_A;
			case XK_B:				return KEY_B;
			case XK_C:				return KEY_C;
			case XK_D:				return KEY_D;
			case XK_E:				return KEY_E;
			case XK_F:				return KEY_F;
			case XK_G:				return KEY_G;
			case XK_H:				return KEY_H;
			case XK_I:				return KEY_I;
			case XK_J:				return KEY_J;
			case XK_K:				return KEY_K;
			case XK_L:				return KEY_L;
			case XK_M:				return KEY_M;
			case XK_N:				return KEY_N;
			case XK_O:				return KEY_O;
			case XK_P:				return KEY_P;
			case XK_Q:				return KEY_Q;
			case XK_R:				return KEY_R;
			case XK_S:				return KEY_S;
			case XK_T:				return KEY_T;
			case XK_U:				return KEY_U;
			case XK_V:				return KEY_V;
			case XK_W:				return KEY_W;
			case XK_X:				return KEY_X;
			case XK_Y:				return KEY_Y;
			case XK_Z:				return KEY_Z;

			case XK_a:				return KEY_A;
			case XK_b:				return KEY_B;
			case XK_c:				return KEY_C;
			case XK_d:				return KEY_D;
			case XK_e:				return KEY_E;
			case XK_f:				return KEY_F;
			case XK_g:				return KEY_G;
			case XK_h:				return KEY_H;
			case XK_i:				return KEY_I;
			case XK_j:				return KEY_J;
			case XK_k:				return KEY_K;
			case XK_l:				return KEY_L;
			case XK_m:				return KEY_M;
			case XK_n:				return KEY_N;
			case XK_o:				return KEY_O;
			case XK_p:				return KEY_P;
			case XK_q:				return KEY_Q;
			case XK_r:				return KEY_R;
			case XK_s:				return KEY_S;
			case XK_t:				return KEY_T;
			case XK_u:				return KEY_U;
			case XK_v:				return KEY_V;
			case XK_w:				return KEY_W;
			case XK_x:				return KEY_X;
			case XK_y:				return KEY_Y;
			case XK_z:				return KEY_Z;

			case XK_0:				return KEY_0;
			case XK_1:				return KEY_1;
			case XK_2:				return KEY_2;
			case XK_3:				return KEY_3;
			case XK_4:				return KEY_4;
			case XK_5:				return KEY_5;
			case XK_6:				return KEY_6;
			case XK_7:				return KEY_7;
			case XK_8:				return KEY_8;
			case XK_9:				return KEY_9;

			case XK_F1:				return KEY_F1;
			case XK_F2:				return KEY_F2;
			case XK_F3:				return KEY_F3;
			case XK_F4:				return KEY_F4;
			case XK_F5:				return KEY_F5;
			case XK_F6:				return KEY_F6;
			case XK_F7:				return KEY_F7;
			case XK_F8:				return KEY_F8;
			case XK_F9:				return KEY_F9;
			case XK_F10:			return KEY_F10;
			case XK_F11:			return KEY_F11;
			case XK_F12:			return KEY_F12;
			case XK_F13:			return KEY_F13;
			case XK_F14:			return KEY_F14;
			case XK_F15:			return KEY_F15;

			case XK_KP_0:			return KEY_NUMPAD0;
			case XK_KP_1:			return KEY_NUMPAD1;
			case XK_KP_2:			return KEY_NUMPAD2;
			case XK_KP_3:			return KEY_NUMPAD3;
			case XK_KP_4:			return KEY_NUMPAD4;
			case XK_KP_5:			return KEY_NUMPAD5;
			case XK_KP_6:			return KEY_NUMPAD6;
			case XK_KP_7:			return KEY_NUMPAD7;
			case XK_KP_8:			return KEY_NUMPAD8;
			case XK_KP_9:			return KEY_NUMPAD9;

			case XK_Num_Lock:       return KEY_NUMLOCK;
			case XK_KP_Enter:		return KEY_NUMPADENTER;
			case XK_KP_Separator:	return KEY_NUMPADCOMMA;
			case XK_KP_Equal:		return KEY_NUMPADEQUALS;
			case XK_KP_Divide:		return KEY_NUMPADSLASH;
			case XK_KP_Multiply:	return KEY_NUMPADSTAR;
			case XK_KP_Delete:		return KEY_NUMPADPERIOD;
			case XK_KP_Decimal:		return KEY_NUMPADPERIOD;
			case XK_KP_Add:         return KEY_NUMPADPLUS;
			case XK_KP_Subtract:    return KEY_NUMPADMINUS;

			case XK_Control_L:		return KEY_LCONTROL;
			case XK_Alt_L:			return KEY_LALT;
			case XK_Shift_L:		return KEY_LSHIFT;
			case XK_Super_L:		return KEY_LWIN;
			case XK_Control_R:		return KEY_RCONTROL;
			case XK_Alt_R:			return KEY_RALT;
			case XK_Shift_R:		return KEY_RSHIFT;
			case XK_Super_R:		return KEY_RWIN;

			case XK_Caps_Lock:		return KEY_CAPSLOCK;

			case XK_bracketleft:	return KEY_LBRACKET;
			case XK_bracketright:	return KEY_RBRACKET;

			case XK_backslash:		return KEY_BACKSLASH;
			case XK_slash:			return KEY_SLASH;
			case XK_colon:			return KEY_COLON;
			case XK_semicolon:		return KEY_SEMICOLON;
			case XK_space:			return KEY_SPACE;
			case XK_comma:			return KEY_COMMA;
			case XK_yen:			return KEY_YEN;
			case XK_at:				return KEY_AT;
			case XK_grave:			return KEY_GRAVE;
			case XK_Insert:			return KEY_INSERT;

			case XK_Kanji:			return KEY_KANJI;
			case XK_underscore:     return KEY_UNDERLINE;
			case XK_minus:			return KEY_MINUS;
			case XK_apostrophe:     return KEY_APOSTROPHE;
			case XK_equal:          return KEY_EQUALS;
			case XK_period:         return KEY_PERIOD;

			// based on some test runs on an ubuntu system with KDE desktop manager
			// don't know whether this independent of the kind of linux system
			case 269025041:			return KEY_VOLUMEDOWN;
			case 269025042:			return KEY_MUTE;
			case 269025043:			return KEY_VOLUMEUP;
			case 269025044:			return KEY_PLAYPAUSE;
			case 269025046:			return KEY_PREVTRACK;
			case 269025047:			return KEY_NEXTTRACK;

			case XK_kana_switch:	return KEY_KANA; // todo right XK_?
			case XK_End:			return KEY_END; // todo right?
			case XK_Menu:			return KEY_MENU; // todo find windows key

			//	todo: unsupported keys

			//		case XK_braceleft:		return {;
			//		case XK_braceright:		return };
			//		case XK_bar:			return |;
			//		case XK_asciitilde:		return ~;

			//		KEY_AX,
			//		KEY_MYCOMPUTER,
			//		KEY_ABNT_C1,
			//		KEY_ABNT_C2,
			//		KEY_APPS,
			//		KEY_WAKE
			//		KEY_STOP,
			//		KEY_OEM_102,
			//		KEY_CALCULATOR,
			//		KEY_CONVERT,
			//		KEY_NOCONVERT
			//		KEY_MEDIASELECT,
			//		KEY_MEDIASTOP,
			//		KEY_SLEEP,
			//		KEY_POWER,
			//		KEY_UNLABELED,
			//		KEY_MAIL,

			//		KEY_WEBBACK,
			//		KEY_WEBFAVORITES,
			//		KEY_WEBFORWARD,
			//		KEY_WEBHOME,
			//		KEY_WEBREFRESH,
			//		KEY_WEBSEARCH,
			//		KEY_WEBSTOP,

			default:
			{
				// todo log this
				break;
			}
		}

		return KEY_FAIL;
	}

#endif // _WINDOWS

void Platform::Window::hideOperatingSystemCursor()
{
	#ifdef _WINDOWS
		// keep cursor within window area where it is hidden thanks to ShowCursor
		RECT windowRectangle;
		GetWindowRect(Window::getSingleton().getWindowHandle(), &windowRectangle);
		ClipCursor(&windowRectangle);
	
		while(ShowCursor(false)>=0);
	#else
		assert(false);
		cerr << "Not implemented: hideOperatingSystemCursor()" << endl;
	#endif
}
	
void Platform::Window::onActivityChange(bool active)
{
	if (active == mActive)
		return;
	else
		mActive = active;

	const uint32 observerCount = getObserverCount();
	if (mActive)
		for (size_t i = 0; i < observerCount; ++i)
			mObservers[i]->onActivation();
	else
		for (size_t i = 0; i < observerCount; ++i)
			mObservers[i]->onDeactivation();
}
