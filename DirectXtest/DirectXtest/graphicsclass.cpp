




///////////////////////////////////
// Filename: graphicsclass.cpp
///////////////////////////////////
#include "graphicsclass.h"
#include <math.h>

//Now that we have a D3DClass member we will start to fill out some code inside the GraphicsClass to initializeand shutdown the D3DClass object.
//We will also add calls to BeginSceneand EndScene in the Render function so that we are now drawing to the window using Direct3D.

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	m_Camera = 0;
	m_Model = 0;

	m_ColorShader = 0;
	m_TextureShader = 0;
	m_MultiTextureShader = 0;

	m_FogShader = 0;

	m_LightShader = 0;
	m_Light = 0;

	m_Bitmap = 0;

	m_Text = 0;

	m_ModelList = 0;
	m_Frustum = 0;

	m_RenderTexture = 0;
	m_DebugWindow = 0;

	baseViewMatrix = XMMatrixIdentity();
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


//The second change is in the Initialize function inside the GraphicsClass.
//Here we create the D3DClass objectand then call the D3DClass Initialize function.
//We send this function the screen width, screen height, handle to the window, and the four global variables from the Graphicsclass.h file.
//The D3DClass will use all these variables to setup the Direct3D system.
//We'll go into more detail about that once we look at the d3dclass.cpp file.

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	

	// Create the Direct3D object.
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}


	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	//We create a new view matrix from the camera object for the TextClass to use.
	//It will always use this view matrix so that the text is always drawn in the same location on the screen.
	
	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	//Here we createand initialize the new TextClass object.
	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}
	// Initialize the text object.
  	result = m_Text->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_Direct3D->GetDevice(),  m_Direct3D->GetDeviceContext(),L"./3DModel/Cube.txt", L"./3DModel/Texture/seafloor.dds", L"./3DModel/Texture/bump01.dds", L"./3DModel/Texture/light01.dds");

	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}



	//// Create the color shader object.
	//m_ColorShader = new ColorShaderClass;
	//if (!m_ColorShader)
	//{
	//	return false;
	//}

	// Initialize the color shader object.
	//result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	/*if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	*/

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the multitexture shader object.
	m_MultiTextureShader = new MultiTextureShaderClass;
	if (!m_MultiTextureShader)
	{
		return false;
	}

	// Initialize the multitexture shader object.
	result = m_MultiTextureShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the multitexture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the fog shader object.
	m_FogShader = new FogShaderClass;
	if (!m_FogShader)
	{
		return false;
	}

	// Initialize the fog shader object.
	result = m_FogShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the fog shader object.", L"Error", MB_OK);
		return false;
	}




	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, L"./Resources/sword.dds", 48, 48);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	
	//The new light object is created here.

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}
	
	//The color of the light is set to purple and the light direction is set to point down the positive Z axis.

	// Initialize the light object.

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	//m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);


	//Here we create the new ModelListClass object and have it create 25 randomly placed/colored sphere models.
	// Create the model list object.
	m_ModelList = new ModelListClass;
	if (!m_ModelList) {
		return false;
	}

	// Initialize the model list object.
	result = m_ModelList->Initialize(1);
	if (!result) {
		MessageBoxW(hwnd, L"Could not initialize the model list object", L"Error", MB_OK);
		return false;
	}

	//Here we create the new FrustumClass object. 
	//It doesn't need any initialization since that is done every frame using the ConstructFrustum function.
	// Create the frustum object.

	m_Frustum = new FrustumClass;
	if (!m_Frustum) {
		return false;
	}


	//We create and initialize the new render to texture object here. 
	//Notice for the size of the texture I send in the screen width and height as I want to render the entire screen evenly to a texture of the same size.
	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	//Here we create and initialize a new debug window object. Notice I have made the window size 100x100. 
	//There will obviously be some distortion since we will be mapping a full screen image down to a 100x100 texture. 
	//To fix the aspect ratio (if it is important for your purposes) then just make sure the debug window is sized smaller but with the same aspect ratio.

	// Create the debug window object.
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// Initialize the debug window object.
	result = m_DebugWindow->Initialize(m_Direct3D->GetDevice(), screenWidth, screenHeight, 100, 100);
	if (!result)
	{
		MessageBoxW(hwnd, L"Could not initialize the debug window object.", L"Error", MB_OK);
		return false;
	}
	return true;
}

//The next change is in the ProgramEnd function in the GraphicsClass.
//Shut down of all graphics objects occur here so we have placed the D3DClass shutdown in this function.
//Note that I check to see if the pointer was initialized or not.
//If it wasn't we can assume it was never set up and not try to shut it down. 
//That is why it is important to set all the pointers to null in the class constructor. 
//If it does find the pointer has been initialized then it will attempt to shut down the D3DClass and then clean up the pointer afterwards.


void GraphicsClass::ProgramEnd()
{

	// Release the debug window object.
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}
	// Release the frustum object.
	if (m_Frustum) {
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model list object.
	if (m_ModelList) {
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}
	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the fog shader object.
	if (m_FogShader)
	{
		m_FogShader->Shutdown();
		delete m_FogShader;
		m_FogShader = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the multitexture shader object.
	if (m_MultiTextureShader)
	{
		m_MultiTextureShader->Shutdown();
		delete m_MultiTextureShader;
		m_MultiTextureShader = 0;
	}

	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}
	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the Direct3D object.
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
	return;
}

//The Frame function has been updated so that it now calls the Render function each frame.

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, int mouseY = 0, int mouseX = 0, float rotationY = 0)
{
	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Set the frames per second.
	result = m_Text->SetFps(fps, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}


	// Set the location of the mouse.
	result = m_Text->SetMousePosition(mouseX, mouseY, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	m_Camera->SetRotation(0.0f, rotationY, 0.0f);


	result = Update();
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render(rotation, mouseX, mouseY);
	if (!result)
	{
		return false;
	}
	return true;
}

//The final change to this class is in the Render function. 
//We call the m_Direct3D object to clear the screen to a grey color. 
//After that we call EndScene so that the grey color is presented to the window.

bool GraphicsClass::Render(float rotation, int mouseX = 0, int mouseY = 0)
{

	XMMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;

	//The first pass of our render is to a texture now.
	// Render the entire scene to the texture first.
	result = RenderToTexture(rotation);
	if (!result)
	{
		return false;
	}

	//The second pass of our render is to the back buffer as normal.
	// Clear the buffers to begin the scene.
	//m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	float fogColor;
	// Set the color of the fog to grey.
	fogColor = 0.5f;


	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	// Render the scene as normal to the back buffer.
	result = RenderScene(rotation);
	if (!result)
	{
		return false;
	}

	// Get the world, view, and ortho matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetOrthoMatrix(orthoMatrix);

	//Then after the rendering is complete we render the 2D debug window so we can see the render to texture as a 2D image at the 50x50 pixel location.
	//The Z buffer is turned off before we do any 2D rendering.
	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	
	

	// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_DebugWindow->Render(m_Direct3D->GetDeviceContext(), 50, 50);
	if (!result)
	{
		return false;
	}

	// Render the debug window using the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, baseViewMatrix,
		orthoMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}


	//We then render the bitmap to the 100, 100 location on the screen.
	//You can change this to wherever you want it rendered.
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_Direct3D->GetDeviceContext(), mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	//Once the vertex/index buffers are prepared we draw them using the texture shader. 
	//Notice we send in the orthoMatrix instead of the projectionMatrix for rendering 2D. 
	//Due note also that if your view matrix is changing you will need to create a default one for 2D rendering and use it instead of the regular view matrix. 
	//In this project using the regular view matrix is fine as the camera in this project is stationary.

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, baseViewMatrix, 
		orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}



	//Here we turn on alpha blending so the text will blend with the background.
	// Turn on the alpha blending before rendering the text.
	m_Direct3D->TurnOnAlphaBlending();

	//We call the text object to render all its sentences to the screen here. 
	//And just like with 2D images we disable the Z buffer before drawing and then enable it again after all the 2D has been drawn.
	// Render the text strings.
	result = m_Text->Render(m_Direct3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}
	// Turn off alpha blending after rendering the text.
	m_Direct3D->TurnOffAlphaBlending();


	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_Direct3D->TurnZBufferOn();


	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();
	return true;
}

bool GraphicsClass::Update() {

	return true;
}

bool GraphicsClass::RenderToTexture(float rotation)
{
	bool result;


	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView());
	//Clear the render to texture background to blue so we can differentiate it from the rest of the normal scene.
	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_Direct3D->GetDeviceContext(), m_Direct3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer.
	result = RenderScene(rotation);
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_Direct3D->SetBackBufferRenderTarget();

	return true;
}

//The RenderScene function is another one of the new private rendering functions.
//It renders the entire scene in one function so we can just set where we want the scene rendered to and then call this function. 
//In this tutorial we call this function once in the RenderToTexture function to render the scene to a texture, and then we call it again in the Render function to render it to the back buffer as normal.
bool GraphicsClass::RenderScene(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix,rotatedWorldMatrix;

	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	XMFLOAT4 color;
	bool renderModel, result;
	float fogStart, fogEnd;

	// Set the start and end of the fog.
	fogStart = 0.0f;
	fogEnd = 5.0f;

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//worldMatrix = XMMatrixRotationY(rotation);
	rotatedWorldMatrix = worldMatrix;
	rotatedWorldMatrix = XMMatrixRotationY(rotation);
	//The major change to the Render function is that we now construct the viewing frustum each frame based on the updated viewing matrix. 
	//This construction has to occur each time the view matrix changes or the frustum culling checks we do will not be correct.

	//Construct the frustum
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	//Get the number of the models that have been rendered
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	//Now loop through all the models in the ModelListClass object.
	// Go through all the models and render them only if they can be seen by the camera view.

	for (index = 0; index < modelCount; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;


		//Here is where we use the new FrustumClass object. 
		//We check if the sphere is viewable in the viewing frustum. 
		//If it can be seen we render it, if it cannot be seen we skip it and check the next one. 
		//This is where we will gain all the speed by using frustum culling.
		// Check if the sphere model is in the view frustum.
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		if (renderModel) {
			// Move the model to the location it should be rendered at.
			//worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			m_Model->Render(m_Direct3D->GetDeviceContext());

			// Render the model with the fog shader.
			result = m_FogShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), rotatedWorldMatrix, viewMatrix, projectionMatrix,
				m_Model->GetTextureArray()[0], fogStart, fogEnd);

			//// Render the model using the multitexture shader.
			//m_MultiTextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			//m_Model->GetTextureArray());

			// Render the model using the light shader.
			//result = m_LightShader->Render(m_Direct3D->GetDeviceContext(),
			//	m_Model->GetIndexCount(),
			//	m_Model->GetTextureArray(),
			//	m_LightShader->GenarateMatrixBuffer(rotatedWorldMatrix, viewMatrix, projectionMatrix),
			//	m_LightShader->GenerateCameraBuffer(m_Camera->GetPosition(), 0.0f),
			//	m_LightShader->GenerateLightBuffer(m_Light->GetAmbientColor(), /*color*/m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_Light->GetSpecularPower(), m_Light->GetSpecularColor())
			//);

			/*if (!result)
			{
				return false;
			}*/

			// Reset to the original world matrix.
			m_Direct3D->GetWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}


	}

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model->Render(m_Direct3D->GetDeviceContext());

	//// Render the model using the color shader.
	//result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);


	// Render the model using the texture shader.
	//result = m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());



	// Render the model using the light shader.
	/*result = m_LightShader->Render(m_Direct3D->GetDeviceContext(),
								   m_Model->GetIndexCount(),
								   m_Model->GetTexture(),
								   m_LightShader->GenarateMatrixBuffer(rotatedWorldMatrix, viewMatrix, projectionMatrix),
								   m_LightShader->GenerateCameraBuffer(m_Camera->GetPosition(), 0.0f),
								   m_LightShader->GenerateLightBuffer(m_Light->GetAmbientColor(),m_Light->GetDiffuseColor(),m_Light->GetDirection(),m_Light->GetSpecularPower(),m_Light->GetSpecularColor())
								   );*/

								   /*if (!result)
								   {
									   return false;
								   }
							   */

	result = m_Text->SetRenderCount(renderCount, m_Direct3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return true;
}

