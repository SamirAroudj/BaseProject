/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#include <GL/glew.h>
#include "Platform/FailureHandling/GraphicsException.h"
#include "Platform/Storage/File.h"
#include "Graphics/GraphicsManager.h"
#include "Graphics/Texture.h"
#include "Platform/Window.h"

using namespace FailureHandling;
using namespace Graphics;
using namespace Platform;
using namespace Storage;
using namespace std;

const char *GraphicsManager::OPENGL_LOG_FILE_NAME = "OpenGL.log";

#ifdef _DEBUG
	void OpenGLErrorCallbackFun(GLenum source, GLenum type, GLuint ID, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
	{
		// output error to file
		{
			// todo log this properly
			// error message
			string text = "Message:\n";
			text += message;
			text += "\n";

			// write to file
			File errorFile(GraphicsManager::OPENGL_LOG_FILE_NAME, File::APPEND_WRITING, false);
			errorFile.write(text.c_str(), sizeof(char), text.size());
		}

		// output error to error output
		{
			cerr << "OpenGL error callback function:";
			cerr << "\nID: " << ID;
			cerr << std::hex;
			cerr << "\ntype: " << type;
			cerr << "\nseverity: " << severity;
			cerr << "\n" << message;
			cerr << endl;
		}

		//throw GraphicsException(message, type);
	}
#endif // _DEBUG

GraphicsManager::GraphicsManager(uint32 colorResolution, const Color &clearColor) : mClearColor(clearColor)
{
	assert(16 == colorResolution|| 32 == colorResolution);

	Platform::Window &window = Platform::Window::getSingleton();

	#ifdef _WINDOWS
		mDeviceContext		= NULL;
		mRenderingContext	= NULL;

		// get device and set up buffer and pixel format
		if (!(mDeviceContext = GetDC(window.getWindowHandle())))
			throw GraphicsException("Could not obtain a device context.", 1);
		if (window.isFullScreen())
			changeDisplaySettings(colorResolution);
		choosePixelFormat(colorResolution);

		// get and activate the GL rendering context
		if (!(mRenderingContext = wglCreateContext(mDeviceContext)))
			throw GraphicsException("Could not obtain a rendering context.", 4);
		if (!wglMakeCurrent(mDeviceContext, mRenderingContext))
			throw GraphicsException("Could not activate the GL rendering context", 5);

	#elif _LINUX
		int colorBitsPerChannel = (16 == colorResolution ? 4 : 8);

		// buffer swap chain properties to define frame, depth & stencil buffer
		GLint bufferSwapChainAttributes[] =
		{
			GLX_RGBA,
			GLX_RED_SIZE, colorBitsPerChannel,
			GLX_GREEN_SIZE, colorBitsPerChannel,
			GLX_BLUE_SIZE, colorBitsPerChannel,
			GLX_ALPHA_SIZE, colorBitsPerChannel,
			GLX_DEPTH_SIZE, window.getDepthBitsPerPixel(),
			GLX_STENCIL_SIZE, window.getStencilBitsPerPixel(),
			GLX_DOUBLEBUFFER, true,
			None
		};

		// get visual info for default screen
		int defaultScreenID = XDefaultScreen(window.getDisplay());

		if (!glXQueryExtension(window.getDisplay(), 0, 0))
		  throw GraphicsException( "X Server doesn't support GLX extension.", -1);

		XVisualInfo *visualInfo = glXChooseVisual(window.getDisplay(), defaultScreenID, bufferSwapChainAttributes);
		if (NULL == visualInfo)
			throw GraphicsException("Depth, stencil or frame buffer format is not supported.", 6);

		// set visual and update / recreate main window
		window.setVisual(*visualInfo);

		// create and bind a GL context for OpenGL without display list sharing
		mRenderingContext = glXCreateContext(window.getDisplay(), visualInfo, NULL, GL_TRUE);
		if (NULL == mRenderingContext)
			throw GraphicsException("Could not create a OpenGL / GLX rendering context.", 7);
		glXMakeCurrent(window.getDisplay(), window.getWindowHandle(), mRenderingContext);

		// free resources
		XFree(visualInfo);
		visualInfo = NULL;
	#endif // _WINDOWS

	// initialize glew for advanced OpenGL functionality such as VBOs
	glewInit();

	// set clear color and clear depth
	setClearColor(clearColor);
	glClearDepth(1.0f);
	glDepthRange(0.0f, 1.0f);

	// enable textures and set the model view matrix to identity
    glMatrixMode(GL_MODELVIEW);													
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);

	#ifdef _DEBUG
		// create clean log file and enable debug output for OpenGL
		const string startMessage = "OpenGL version string: ";
		const GLubyte *version = glGetString(GL_VERSION);

		File file(OPENGL_LOG_FILE_NAME, File::CREATE_WRITING, false);
		file.write(startMessage.c_str(), sizeof(char), startMessage.length());
		file.write(version, sizeof(GLubyte), strlen((const char *) version));
		file.write("\n", sizeof(char), 1);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback((GLDEBUGPROC) OpenGLErrorCallbackFun, NULL);
	#endif // _DEBUG
}

#ifdef _WINDOWS
	void GraphicsManager::changeDisplaySettings(uint32 colorResolution)
	{
		const Window &window = Window::getSingleton();
		const bool fullscreen = window.isFullScreen();

		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));

		screenSettings.dmSize		= sizeof(screenSettings);
		screenSettings.dmPelsWidth	= (fullscreen ? window.getScreenSize()[0] : window.getSize()[0]);
		screenSettings.dmPelsHeight	= (fullscreen ? window.getScreenSize()[1] : window.getSize()[1]);
		screenSettings.dmBitsPerPel	= colorResolution;
		screenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (DISP_CHANGE_SUCCESSFUL != ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN))
			throw GraphicsException("Fullscreen mode is not supported.", 0);
	}

	void GraphicsManager::choosePixelFormat(uint32 colorResolution)
	{
		PIXELFORMATDESCRIPTOR pixelFormatDescription;
		GLuint pixelFormat;

		// configure pixel format
		memset(&pixelFormatDescription, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pixelFormatDescription.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
		pixelFormatDescription.nVersion		= 1;
		pixelFormatDescription.dwFlags		= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pixelFormatDescription.iPixelType	= PFD_TYPE_RGBA;
		pixelFormatDescription.cColorBits	= (16 == colorResolution) ? 12 : 24;
		pixelFormatDescription.cAlphaBits	= (16 == colorResolution) ? 4  : 8;
		pixelFormatDescription.cDepthBits	= static_cast<BYTE>(Window::getSingleton().getDepthBitsPerPixel());
		pixelFormatDescription.cStencilBits	= static_cast<BYTE>(Window::getSingleton().getStencilBitsPerPixel());
		pixelFormatDescription.iLayerType	= PFD_MAIN_PLANE;
	
		// apply pixel format
		if (!(pixelFormat = ChoosePixelFormat(mDeviceContext, &pixelFormatDescription)))
			throw GraphicsException("Could not find a suitable pixel format.", 2);
		if (!SetPixelFormat(mDeviceContext, pixelFormat, &pixelFormatDescription))
			throw GraphicsException("Could not set the chosen pixel format.", 3);
	}
#endif // _WINDOWS

GraphicsManager::~GraphicsManager()
{
	// free render groups
	size_t numOfRenderGroups = mRenderGroups.size();	
	for (size_t i = 0; i < numOfRenderGroups; ++i)
		delete mRenderGroups[i];

	// free texture memory
	Texture::freeMemory();

	// free rendering context & devices
	Platform::Window &window = Platform::Window::getSingleton();

	#ifdef _WINDOWS
		// show cursor
		ShowCursor(true);

		// back to desktop, release device and rendering context
		if (window.isFullScreen())
			ChangeDisplaySettings(NULL, 0);
		wglMakeCurrent(NULL, NULL);
		if (mRenderingContext)
			wglDeleteContext(mRenderingContext);
		if (mDeviceContext)
			ReleaseDC(window.getWindowHandle(), mDeviceContext);

	#elif _LINUX
		// unbind & release glx context
		glXMakeCurrent(window.getDisplay(), None, NULL);
		glXDestroyContext(window.getDisplay(), mRenderingContext);

	#endif // _WINDOWS
}

void GraphicsManager::checkProgram(const uint32 programIdx) const
{
	// check program
	glValidateProgram(programIdx);

	// valid program?
	GLint valid = GL_FALSE;
	glGetProgramiv(programIdx, GL_VALIDATE_STATUS, &valid);
	if (GL_TRUE == valid)
		return;

	// get link status & log entry on failure
	GLint linked = GL_FALSE;
	glGetProgramiv(programIdx, GL_LINK_STATUS, &linked);
	if (GL_TRUE != linked)
	{
		// todo log this properly
		cerr << "Program could not be linked!" << endl;
	}

	// get program log length
	GLint infoLogLength = 0;
	glGetProgramiv(programIdx, GL_INFO_LOG_LENGTH, &infoLogLength);

	// get program log
	GLsizei actualLength;
	char *log = new char[infoLogLength];
	glGetProgramInfoLog(programIdx, infoLogLength, &actualLength, log);

	// output log
	{
		string message = "GraphicsManager::checkProgram:\n";

		// write to file
		File errorFile(GraphicsManager::OPENGL_LOG_FILE_NAME, File::APPEND_WRITING, false);
		errorFile.write(message.c_str(), sizeof(char), message.length());
		errorFile.write(log, sizeof(char), infoLogLength);
	}

	// cerr & exception
	cerr << "glGetProgramInfoLog:\n";
	cerr << log << endl;
	GraphicsException exception(log, programIdx);

	delete [] log;
	log = NULL;

	throw exception;
}

void GraphicsManager::checkShader(const uint32 shaderID) const
{
	// compiled?
	GLint compiled = GL_FALSE;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (GL_TRUE == compiled)
		return;

	// get shader log length
	GLint infoLogLength = 0;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	// get shader log
	GLsizei actualLength;
	char *log = new char[infoLogLength];
	glGetShaderInfoLog(shaderID, infoLogLength, &actualLength, log);

	// output log
	// todo log this porperly
	{
		string message = "GraphicsManager::checkShader:\n";

		// write to file
		File errorFile(GraphicsManager::OPENGL_LOG_FILE_NAME, File::APPEND_WRITING, false);
		errorFile.write(message.c_str(), sizeof(char), message.length());
		errorFile.write(log, sizeof(char), infoLogLength);
	}

	// cerr & exception
	cerr << "glGetShaderInfoLog:\n";
	cerr << log << endl;
	GraphicsException exception(log, shaderID);

	delete [] log;
	log = NULL;

	throw exception;
}

void GraphicsManager::clearBackAndDepthStencilBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();
	//glTranslater(0.0f, 0.0f, ORTHO_PROJECTION_Z_NEAR);
}

void GraphicsManager::createRenderGroups(uint32 numOfRenderGroupsInTheEnd)
{
	assert(numOfRenderGroupsInTheEnd >= mRenderGroups.size());
	for(size_t i = mRenderGroups.size(); i < numOfRenderGroupsInTheEnd; ++i)
		mRenderGroups.push_back(new RenderGroup());
}

void GraphicsManager::disableBlending() const
{
	glDisable(GL_BLEND);
}

void GraphicsManager::enableAlphaBlending() const
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

RenderGroup &GraphicsManager::getRenderGroup(size_t number)
{
	// valid number?
	const size_t numOfGroups = mRenderGroups.size();														
	assert(number <= numOfGroups);
	assert(number >= 0);

	// create new render group
	if (numOfGroups == number)																				
	{
		mRenderGroups.push_back(new RenderGroup());
		return *mRenderGroups.back();
	}

	// return existing group
	return *(mRenderGroups[number]);																		
}

void GraphicsManager::presentBackBuffer()
{
	#ifdef _WINDOWS
		SwapBuffers(mDeviceContext);

	#elif _LINUX
		Platform::Window &window = Platform::Window::getSingleton();
		glXSwapBuffers(window.getDisplay(), window.getWindowHandle());

	#endif // _WINDOWS
}

void GraphicsManager::renderRenderGroups()
{
	glMatrixMode(GL_MODELVIEW);																		// set model view matrix, depth test and function
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	size_t numOfRenderGroups = mRenderGroups.size();
	for (size_t i = numOfRenderGroups - 1; i >= 0; --i)
		mRenderGroups[i]->render();
}

void GraphicsManager::setClearColor(const Color &color)
{
	mClearColor = color;
	glClearColor((float) mClearColor.getRed(),
				 (float) mClearColor.getGreen(),
				 (float) mClearColor.getBlue(),
				 (float) mClearColor.getAlpha());
}
