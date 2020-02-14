﻿////////////////////////////
// Filename: d3dclass.cpp
////////////////////////////
#include "d3dclass.h"


//So like most classes we begin with initializing all the member pointers to null in the class constructor.
//All pointers from the header file have all been accounted for here.

D3DClass::D3DClass()
{
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
	m_rasterStateNoCulling = nullptr;
	m_rasterStateWireframe = nullptr;
	m_depthDisabledStencilState = nullptr;
	m_alphaEnableBlendingState = nullptr;
	m_alphaDisableBlendingState = nullptr;
	m_alphaEnableBlendingState2 = nullptr;
	m_alphaEnableBlendingStateParticle = nullptr;
}


D3DClass::D3DClass(const D3DClass& other)
{
}


D3DClass::~D3DClass()
{
}

//The Initialize function is what does the entire setup of Direct3D for DirectX 11. 
//I have placed all the code necessary in here as well as some extra stuff that will facilitate future projects.
//I could have simplified it and taken out some items but it is probably better to get all of this covered in a single project dedicated to it.

//The screenWidth and screenHeight variables that are given to this function are the width and height of the window we created in the SystemClass.Direct3D will use these to initialize and use the same window dimensions.
//The hwnd variable is a handle to the window.
//Direct3D will need this handle to access the window previously created.
//The fullscreen variable is whether we are running in windowed mode or fullscreen.
//Direct3D needs this as well for creating the window with the correct settings.
//The screenDepthand screenNear variables are the depth settings for our 3D environment that will be rendered in the window.
//The vsync variable indicates if we want Direct3D to render according to the users monitor refresh rate or to just go as fast as possible.

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen,
	float screenDepth, float screenNear)
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView, screenAspect;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_BLEND_DESC blendStateDescription;
	
	
	// Store the vsync setting.
	m_vsync_enabled = vsync;

	//Before we can initialize Direct3D we have to get the refresh rate from the video card / monitor.
	//Each computer may be slightly different so we will need to query for that information.
	//We query for the numeratorand denominator valuesand then pass them to DirectX during the setupand it will calculate the proper refresh rate.
	//If we don't do this and just set the refresh rate to a default value which may not exist on all computers 
	//then DirectX will respond by performing a blit instead of a buffer flip 
	//which will degrade performance and give us annoying errors in the debug output.

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
	{
		return false;
	}

	// Now fill the display mode list structures.
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	//We now have the numeratorand denominator for the refresh rate.
	//The last thing we will retrieve using the adapter is the name of the video cardand the amount of video memory.
	// Get the adapter (video card) description.

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	//Now that we have stored the numeratorand denominator for the refresh rate and the video card information 
	//We can release the structuresand interfaces used to get that information.

	// Release the display mode list.
	delete[] displayModeList;
	displayModeList = 0;

	// Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	//Now that we have the refresh rate from the system we can start the DirectX initialization.
	//The first thing we'll do is fill out the description of the swap chain. 
	//The swap chain is the front and back buffer to which the graphics will be drawn. 
	//Generally you use a single back buffer, do all your drawing to it, and then swap it to the front buffer which then displays on the user's screen.
	//That is why it is called a swap chain.
	
	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//The next part of the description of the swap chain is the refresh rate.
	//The refresh rate is how many times a second it draws the back buffer to the front buffer.
	//If vsync is set to true in our graphicsclass.h header then this will lock the refresh rate to the system settings(for example 60hz).
	//That means it will only draw the screen 60 times a second(or higher if the system refresh rate is more than 60).
	//However if we set vsync to false then it will draw the screen as many times a second as it can, 
	//however this can cause some visual artifacts.

	// Set the refresh rate of the back buffer.
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	//After setting up the swap chain description we also need to setup one more variable called the feature level.
	//This variable tells DirectX what version we plan to use.
	//Here we set the feature level to 11.0 which is DirectX 11. 
	//You can set this to 10 or 9 to use a lower level version of DirectX if you plan on supporting multiple versions or running on lower end hardware.

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	//	//機能レベルの設定
//	D3D_FEATURE_LEVEL featureLevels[4] =
//	{
//		D3D_FEATURE_LEVEL_11_0,
//		D3D_FEATURE_LEVEL_10_1,
//		D3D_FEATURE_LEVEL_10_0,
//		D3D_FEATURE_LEVEL_9_3
//	};


	/*Now that the swap chain descriptionand feature level have been filled out we can create 
	  the swap chain, the Direct3D device, and the Direct3D device context.
	  The Direct3D deviceand Direct3D device context are very important, 
	  they are the interface to all of the Direct3D functions.
	  We will use the deviceand device context for almost everything from this point forward.

	  Those of you reading this who are familiar with the previous versions of DirectX will recognize the 「Direct3D device」 
	  but will be unfamiliar with the new 「Direct3D device context」.
	  Basically they took the functionality of the Direct3D deviceand split it up into two different devices so you need to use both now.

	  Note that if the user does not have a DirectX 11 video card 
	  this function call will fail to create the device and device context.

	  Also if you are testing DirectX 11 functionality yourself and don't have a DirectX 11 video card 
	  then you can replace 「D3D_DRIVER_TYPE_HARDWARE」 with 「D3D_DRIVER_TYPE_REFERENCE」
	  and DirectX will use your CPU to draw instead of the video card hardware. 
	  Note that this runs 1/1000 the speed but it is good for people who don't have DirectX 11 video cards yet on all their machines.*/

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
										   D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	/*Sometimes this call to create the device will fail 
	  if the primary video card is not compatible with DirectX 11. 
	  Some machines may have the primary card as a DirectX 10 video card 
	  and the secondary card as a DirectX 11 video card.
	  Also some hybrid graphics cards work that way with the primary being the low power Intel card 
	  and the secondary being the high power Nvidia card.
	  To get around this you will need to not use the default deviceand 
	  instead enumerate all the video cards in the machineand have the user choose 
	  which one to useand then specify that card when creating the device.*/

	 //Now that we have a swap chain we need to get a pointer to the back buffer 
	 //and then attach it to the swap chain.
	 //We'll use the CreateRenderTargetView function to attach the back buffer to our swap chain.

	// Get the pointer to the back buffer.
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	//We will also need to set up a depth buffer description.
	//We'll use this to create a depth buffer so that our polygons can be rendered properly in 3D space. 
	//At the same time we will attach a stencil buffer to our depth buffer. 
	//The stencil buffer can be used to achieve effects such as motion blur, volumetric shadows, and other things.

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Now we create the depth / stencil buffer using that description.
	//You will notice we use the 「CreateTexture2D」 function to make the buffers, 
	//hence the buffer is just a 2D texture.
	//The reason for this is that once your polygons are sortedand then rasterized 
	//they just end up being colored pixels in this 2D buffer.
	//Then this 2D buffer is drawn to the screen.

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}
	
	//Now we need to setup the depth stencil description.
	//This allows us to control what type of depth test Direct3D will do for each pixel.

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the LessEqual stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	depthStencilDesc.StencilEnable = false;

	// Create the LessEqual depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilStateLessEqual);
	if (FAILED(result))
	{
		return false;
	}
	
	// Clear the depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//With the description filled out we can now create a depth stencil state.
	
	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	//With the created depth stencil state we can now set it so that it takes effect.
	//Notice we use the device context to set it.
	
	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//The next thing we need to create is the description of the view of the depth stencil buffer.
	//We do this so that Direct3D knows to use the depth buffer as a depth stencil texture.
	//After filling out the description we then call the function CreateDepthStencilView to create it.

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	//With that created we can now call OMSetRenderTargets.
	//This will bind the render target viewand the depth stencil buffer to the output render pipeline.
	//This way the graphics that the pipeline renders will get drawn to our back buffer that we previously created.
	//With the graphics written to the back buffer we can then swap it to the frontand display our graphics on the user's screen.

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	//Now that the render targets are setup we can continue on to some extra functions that will give us more control over our scenes for future projects.
	//First thing is we'll create is a rasterizer state. 
	//This will give us control over how polygons are rendered. 
	//We can do things like make our scenes render in wireframe mode or have DirectX draw both the front and back faces of polygons. 
	//By default DirectX already has a rasterizer state set up and working the exact same as the one below but you have no control to change it unless you set up one yourself.

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK /*D3D11_CULL_NONE*/;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID /*D3D11_FILL_WIREFRAME*/;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;


	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result))
	{
		return false;
	}

	
	//SetRasterizerState(false);

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup a raster description which turns off back face culling.
	rasterDesc.CullMode = D3D11_CULL_NONE;

	// Create the no culling rasterizer state.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateNoCulling);
	if (FAILED(result))
	{
		return false;
	}

	// Setup a raster description which enables wire frame rendering.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the wire frame rasterizer state.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateWireframe);
	if (FAILED(result))
	{
		return false;
	}



	//The viewport also needs to be setup so that Direct3D can map clip space coordinates to the render target space.
	//Set this to be the entire size of the window.

	// Setup the viewport for rendering.
	m_viewport.Width = (float)screenWidth;
	m_viewport.Height = (float)screenHeight;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	//Now we will create the projection matrix.
	//The projection matrix is used to translate the 3D scene into the 2D viewport space that we previously created.
	//We will need to keep a copy of this matrix so that we can pass it to our shaders that will be used to render our scenes.

	// Setup the projection matrix.
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	//We will also create another matrix called the world matrix.
	//This matrix is used to convert the vertices of our objects into vertices in the 3D scene.
	//This matrix will also be used to rotate, translate, and scale our objects in 3D space.
	//From the start we will just initialize the matrix to the identity matrix and keep a copy of it in this object.
	//The copy will be needed to be passed to the shaders for rendering also.
	
	
	// Initialize the world matrix to the identity matrix.
	m_worldMatrix = XMMatrixIdentity();

	/*This is where you would generally create a view matrix.
	  The view matrix is used to calculate the position of where we are looking at the scene from.
	  You can think of it as a camera and you only view the scene through this camera.
	  Because of its purpose I am going to create it in a camera class in later projects since logically it fits better thereand just skip it for now.

	  And the final thing we will setup in the Initialize function is an orthographic projection matrix.
	  This matrix is used for rendering 2D elements like user interfaces on the screen allowing us to skip the 3D rendering.
	  You will see this used in later projects when we look at rendering 2D graphics and fonts to the screen.*/

	// Create an orthographic projection matrix for 2D rendering.
	m_orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result))
	{
		return false;
	}

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	//To create an alpha enabled blend state description change BlendEnable to TRUE and DestBlend to D3D11_BLEND_INV_SRC_ALPHA.
	//The other settings are set to their default values which can be looked up in the Windows DirectX Graphics Documentation.
	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//We then create an alpha enabled blending state using the description we just setup.
	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	//Now to create an alpha disabled state we change the description we just made to set BlendEnable to FALSE.
	//The rest of the settings can be left as they are.
	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	blendStateDescription.AlphaToCoverageEnable = false;
	//We then create an alpha disabled blending state using the modified blend state description. 
	//We now have two blending states we can switch between to turn on and off alpha blending.
	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result))
	{
		return false;
	}

	// Create a blend state description for the alpha-to-coverage blending mode.
	blendStateDescription.AlphaToCoverageEnable = true;
	blendStateDescription.IndependentBlendEnable = false;
	blendStateDescription.RenderTarget[0].BlendEnable = true;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState2);
	if (FAILED(result))
	{
		return false;
	}


	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingStateParticle);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

//The Shutdown function will releaseand clean up all the pointers used in the Initialize function,
//its pretty straight forward.
//However before doing that I put in a call to force the swap chain to go into windowed mode first before releasing any pointers.
//If this is not done and you try to release the swap chain in full screen mode it will throw some exceptions.
//So to avoid that happening we just always force windowed mode before shutting down Direct3D.

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_alphaEnableBlendingState)
	{
		m_alphaEnableBlendingState->Release();
		m_alphaEnableBlendingState = 0;
	}

	if (m_alphaDisableBlendingState)
	{
		m_alphaDisableBlendingState->Release();
		m_alphaDisableBlendingState = 0;
	}

	if (m_alphaEnableBlendingState2)
	{
		m_alphaEnableBlendingState2->Release();
		m_alphaEnableBlendingState2 = 0;
	}

	if (m_alphaEnableBlendingStateParticle)
	{
		m_alphaEnableBlendingStateParticle->Release();
		m_alphaEnableBlendingStateParticle = 0;
	}


	if (m_rasterStateWireframe)
	{
		m_rasterStateWireframe->Release();
		m_rasterStateWireframe = 0;
	}

	if (m_rasterStateNoCulling)
	{
		m_rasterStateNoCulling->Release();
		m_rasterStateNoCulling = 0;
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = 0;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilStateLessEqual)
	{
		m_depthStencilStateLessEqual->Release();
		m_depthStencilStateLessEqual = 0;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}

//In the D3DClass I have a couple helper functions.
//The first two are BeginScene and EndScene.
//BeginScene will be called whenever we are going to draw a new 3D scene at the beginning of each frame.
//All it does is initializes the buffers so they are blank and ready to be drawn to.


void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

//The other function is Endscene, 
//it tells the swap chain to display our 3D scene once all the drawing has completed at the end of each frame.

void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

	return;
}

//These next functions simply get pointers to 
//the Direct3D deviceand the Direct3D device context.
//These helper functions will be called by the framework often.

ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}

//The next three helper functions give copies of 
//the projection, world, and orthographic matrices 
//to calling functions.
//Most shaders will need these matrices for rendering 
//so there needed to be an easy way for outside objects to get a copy of them.
//We won't call these functions in this project but I'm just explaining why they are in the code.

void D3DClass::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
	return;
}


void D3DClass::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
	return;
}


void D3DClass::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

//The last helper function returns by reference the name of the video card and the amount of video memory. 
//Knowing the video card name can help in debugging on different configurations.

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
	return;
}

void D3DClass::TurnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
	return;
}


void D3DClass::TurnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
	return;
}

void D3DClass::TurnDepthStencilStateLessEqual()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilStateLessEqual, 1);
	return;
}




//The first new function TurnOnAlphaBlending allows us to turn on alpha blending by using the OMSetBlendState function with our m_alphaEnableBlendingState blending state.
void D3DClass::TurnOnAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);

	return;
}

//The second new function TurnOffAlphaBlending allows us to turn off alpha blending by using the OMSetBlendState function with our m_alphaDisableBlendingState blending state.
void D3DClass::TurnOffAlphaBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

void D3DClass::TurnOnParticleBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingStateParticle, blendFactor, 0xffffffff);

	return;
}

void D3DClass::TurnOffParticleBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);

	return;
}

//I will just cover the functions that have changed in this class since the previous tutorial.
ID3D11DepthStencilView * D3DClass::GetDepthStencilView()
{
	return m_depthStencilView;
}

//The SetBackBufferRenderTarget function will set the back buffer in this class as the current render target.
//This will usually be called after the render to texture has completed and we want to render the scene to the back buffer again.
void D3DClass::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	return;
}

void D3DClass::ResetViewport()
{
	// Set the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);

	return;

}


void D3DClass::TurnOnCulling()
{
	// Set the culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	return;
}


void D3DClass::TurnOffCulling()
{
	// Set the no back face culling rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateNoCulling);

	return;
}


void D3DClass::EnableAlphaToCoverageBlending()
{
	float blendFactor[4];


	// Setup the blend factor.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState2, blendFactor, 0xffffffff);

	return;
}


void D3DClass::EnableWireframe()
{
	// Set the wire frame rasterizer state.
	m_deviceContext->RSSetState(m_rasterStateWireframe);

	return;
}


void D3DClass::DisableWireframe()
{
	// Set the solid fill rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	return;
}


