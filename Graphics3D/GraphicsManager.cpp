/*
 * Copyright (C) 2017 by Author: Aroudj, Samir, born in Suhl, Thueringen, Germany
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD 3-Clause license. See the License.txt file for details.
 */
#ifdef _DEBUG
#include <sstream>
#endif // _DEBUG

#include "DXGI.h"
#include "Graphics/GraphicsManager.h"
#include "MagicGraphicsConstants.h"
#include "Platform/ResourceManagement/ResourceManagement.h"

using namespace Graphics;
using namespace Platform;
using namespace std;

GraphicsManager::GraphicsManager(uint32 refreshRateHertz, uint32 multisamplingCount, uint32 multisamplingQuality, bool enableVSynchronization) :
	mDevice(NULL),
	mSwapChain(NULL),
	mBackBufferView(NULL),
	mDepthStencilBuffer(NULL),
	mDepthStencilBufferView(NULL),
	mWaitForVSync(enableVSynchronization)
{
#ifdef _DEBUG
	uint32 flags = D3D10_CREATE_DEVICE_DEBUG;
#else
	uint32 flags = 0;
#endif // _DEBUG
	ViewportDescription mainViewport(0, 0, Window::getSingleton().getWidth(), Window::getSingleton().getHeight());
	IDXGIFactory *GIFactory;

	CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&GIFactory));

#ifdef _DEBUG
	outputDisplayModes(GIFactory);
#endif // _DEBUG

	/// default display adapter, use hardware acceleration, no software rasterizer, flags specified above, DirectX 10
	D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, flags, D3D10_SDK_VERSION, &mDevice);
	createSwapChain(GIFactory, refreshRateHertz, multisamplingCount, multisamplingQuality);
	createBackAndDepthStencilBuffers(multisamplingCount, multisamplingQuality);

	setViewports(&mainViewport, 1);
	// disable switching between fullscreen and window mode by alt + enter user input
	GIFactory->MakeWindowAssociation(Window::getSingleton().getWindowHandle(), DXGI_MWA_NO_WINDOW_CHANGES);
	GIFactory->Release();
}

void GraphicsManager::createSwapChain(IDXGIFactory *GIFactory, uint32 refreshRateHertz, uint32 multisamplingCount, uint32 multisamplingQuality)
{
	/// create and fill the swap chain description to configure back buffer, multisampling etc.
	DXGI_SAMPLE_DESC multisamplingDescription;
	multisamplingDescription.Count = 1;

	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	swapChainDescription.BufferCount = 1;

	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// back buffer width, height format, refresh rate etc
	swapChainDescription.BufferDesc.Height = Window::getSingleton().getHeight();
	swapChainDescription.BufferDesc.RefreshRate.Numerator = refreshRateHertz;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1u;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Width = Window::getSingleton().getWidth();

	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // when switching to fullscreen a display mode that fits best to the back buffer is used
	swapChainDescription.OutputWindow = Window::getSingleton().getWindowHandle();

	swapChainDescription.SampleDesc.Count = multisamplingCount;		// multisampling settings
	swapChainDescription.SampleDesc.Quality = multisamplingQuality;

	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDescription.Windowed = !Window::getSingleton().isFullscreen();

	GIFactory->CreateSwapChain(mDevice, &swapChainDescription, &mSwapChain);
}

void GraphicsManager::createBackAndDepthStencilBuffers(uint32 multisamplingCount, uint32 multisamplingQuality)
{
	/// create the back buffer view
	ID3D10Texture2D *backBuffer = NULL;
	mSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void **>(&backBuffer));
	mDevice->CreateRenderTargetView(backBuffer, NULL, &mBackBufferView);
	backBuffer->Release();
	backBuffer = NULL;

	/// create a depth/stencil buffer description
	D3D10_TEXTURE2D_DESC depthStencilBufferDescription;
	depthStencilBufferDescription.ArraySize = 1;
	depthStencilBufferDescription.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthStencilBufferDescription.CPUAccessFlags = 0;	// CPU does not need to access this ressource
	depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 24 Realing point bits [0.0f - 1.0f] for depth buffer and 0 - 255 for stencil buffer
	depthStencilBufferDescription.Height = Window::getSingleton().getHeight();
	depthStencilBufferDescription.MipLevels = 1;
	depthStencilBufferDescription.MiscFlags = 0;
	depthStencilBufferDescription.SampleDesc.Count = multisamplingCount;
	depthStencilBufferDescription.SampleDesc.Quality = multisamplingQuality;
	depthStencilBufferDescription.Usage = D3D10_USAGE_DEFAULT;
	depthStencilBufferDescription.Width = Window::getSingleton().getWidth();

	/// create the depth / stencil buffer and its corresponding view
	mDevice->CreateTexture2D(&depthStencilBufferDescription, 0, &mDepthStencilBuffer);
	mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilBufferView);

	/// bind the back buffer and the depth / stencil buffer to Output Merger stage of the pipeline
	mDevice->OMSetRenderTargets(1, &mBackBufferView, mDepthStencilBufferView);
}

GraphicsManager::~GraphicsManager()
{
	mDepthStencilBufferView->Release();
	mDepthStencilBuffer->Release();
	mBackBufferView->Release();

	mSwapChain->SetFullscreenState(false, NULL);
 	mSwapChain->Release();

	mDevice->ClearState();
	mDevice->Release();
}

void GraphicsManager::clearBackAndDepthStencilBuffer()
{
	mDevice->ClearRenderTargetView(mBackBufferView, BACK_BUFFER_CLEAR_COLOR.getComponents());
	mDevice->ClearDepthStencilView(mDepthStencilBufferView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, DEPTH_BUFFER_MAX_DEPTH, STENCIL_BUFFER_CLEAR_VALUE);
}

void GraphicsManager::clearOutputMergerStates()
{
	Real blendFactors[] = {0.0f, 0.0f, 0.0f, 0.0f};
	mDevice->OMSetDepthStencilState(0, 0);
	mDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
}

uint32 GraphicsManager::getMaxMultisamplingQualityLevel(DXGI_FORMAT backBufferFormat, uint32 multisamplingCount) const
{
	uint32 maxQualityLevel = 0;
	mDevice->CheckMultisampleQualityLevels(backBufferFormat, multisamplingCount, &maxQualityLevel);
	return maxQualityLevel - 1;
}

void GraphicsManager::presentBackBuffer()
{
	mSwapChain->Present(mWaitForVSync, 0);
}

void GraphicsManager::setVertexLayout(const VertexLayout *vertexLayout)
{
	mDevice->IASetInputLayout(vertexLayout->getD3DVertexLayout());
}

void GraphicsManager::setViewports(const ViewportDescription *viewportDescriptions, uint32 numOfDescriptions)
{
	D3D10_VIEWPORT *viewports = new D3D10_VIEWPORT[numOfDescriptions];
	for(uint32 i = 0; i < numOfDescriptions; ++i)
	{
		viewports[i].Height = viewportDescriptions[i].getHeight();
		viewports[i].MaxDepth = viewportDescriptions[i].getMaxDepth();
		viewports[i].MinDepth = viewportDescriptions[i].getMinDepth();
		viewports[i].TopLeftX = viewportDescriptions[i].getTopLeftX();
		viewports[i].TopLeftY = viewportDescriptions[i].getTopLeftY();
		viewports[i].Width = viewportDescriptions[i].getWidth();
	}

	mDevice->RSSetViewports(numOfDescriptions, viewports);
	delete [] viewports;
}

#ifdef _DEBUG
void GraphicsManager::outputDisplayModes(IDXGIFactory *GIFactory)
{
	wostringstream os(wostringstream::out);
	vector<IDXGIAdapter *> adapters;
	vector<IDXGIOutput *> outputs;

	// output all display modes of each output of each adapter as long as its format is equal to the specified format
	enumerateAdapters(GIFactory, false, adapters);
	for(uint32 i = 0; i < adapters.size(); ++i)	// adapters
	{
		OutputDebugString(L"Adapter:\n");
		enumerateOutputs(adapters[i], outputs);
		for(uint32 j = 0; j < outputs.size(); ++j)	// adapter's outputs
		{
			OutputDebugString(L"output's display modes:\n");
			uint32 numberOfDisplayModes = 0;
			outputs[i]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numberOfDisplayModes, NULL);
			DXGI_MODE_DESC *displayModeDescriptions = new DXGI_MODE_DESC[numberOfDisplayModes];
			outputs[i]->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numberOfDisplayModes, displayModeDescriptions);

			for(uint32 k = 0; k < numberOfDisplayModes; ++k)	// output's display modes
			{
				os.clear();
				os << L"Resolution: " << displayModeDescriptions[k].Width << L" x " << displayModeDescriptions[k].Height <<
					L"\tRefresh rate: " << displayModeDescriptions[k].RefreshRate.Numerator << L"/" << displayModeDescriptions[k].RefreshRate.Denominator << "\n";
				OutputDebugString(os.str().c_str());
			}

			delete [] displayModeDescriptions;
			outputs[i]->Release();
		}
		outputs.clear();
		adapters[i]->Release();
	}
}
#endif // _DEBUG

void GraphicsManager::enumerateAdapters(IDXGIFactory *GIFactory, bool directX10Capable, vector<IDXGIAdapter *> &adapters)
{
	IDXGIAdapter *adapter; 
	for(uint32 i = 0; GIFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i) 
	{
		if (directX10Capable && (DXGI_ERROR_UNSUPPORTED == adapter->CheckInterfaceSupport(__uuidof(ID3D10Device), NULL)))
			adapter->Release();
		else
			adapters.push_back(adapter); 
	}
}

void GraphicsManager::enumerateOutputs(IDXGIAdapter * adapter, vector<IDXGIOutput *> &outputs)
{
	IDXGIOutput *output;
	for(uint32 i = 0; adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; ++i)
		outputs.push_back(output);
}
