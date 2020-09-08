#pragma once

//説明(Description):
/*骨組み(Framework):

//予定:


//			WinMain
				|
				|
			Engine<----------------<Utility,StringHelper,ipch,Com,DebugHelper,Timer
				|<-----ImGUI
		________|______________________________________________________________________________________________________________________________________________________________
		|				|				|					  							|											|											ResourceManager
	Graphic			 Input			  Sound	    									Scene											Model<<<<<AssimpLoader,originalloader
		|<-----FrameRender()															|<------FrameUpdate()						|
	____|____________________________________________________________					|					________________________|______________________________________________
	|					|								|			Renderqueue			|			Mesh<<<<VertexType,VertexBuffer,IndexBUffer			Material		Bone	Animation	
	D3D				BuiltInShader<<<ConstBufferType		Effects			|				|			Texture,Color			      |
	|					|											MeshRenderer		|										  |
	|				VertexShader,PixelShader(2D,3D)										|										  |
	RenderState																			|									      |
																						|									      |
																						l									      |
																					Component<<<<<<Transform    		          |
																			____________|________________					      |
																		Camera						GameObject<<Update()	      |
																										|					      |
																							____________|___________		      |
																						2DObjct					3DObject<------

																						
*/

/*

初期案:
//参考:http://rastertek.com/dx11s2tut02.html

							WinMain
							   |
							   |
						  SystemClass
							   |
				  _____________|____________________________________________________________________________________________________________________________________
				 |					 |							|							|						|						|						|
				 |					 |							|							|						|						|						|
			InputClass			GraphicClass				SoundClass					FPSClass				CPUClass				TimerClass				Transform
									 |
		  ___________________________|___________________________________________________________________________________________________________________________________________________________________________________
		  |				|		        |                 |						    |                   |				|				       |					    |						|        				|	
		D3DClass     FrustumClass   ModelClass        ModelListClass            CameraClass        	LightClass		BitmapClass   		  TextClass				RenderTextureClass		DebugWindowClass			ShaderManagerClass---------------------------------->ColorShaderClass	   TextureShaderClass    LightShaderClass      MultiTexShaderClass		FogShaderClass	...
|										|																				|				       |
|										|																				|			    _______|_______
                                 TextureArrayClass																	TextureClass		|			  |
																																		|			  |	
																																	FontClass	FontShaderClass
																																		|
																																		|
																																	TextureClass



Terrain:


							WinMain
							   |
							   |
						  SystemClass
							   |
							   |
						ApplicationClass
							   |
				  _____________|____________________________________________________________________________________________________________________________________
				 |					 |							|							|						|						|						|
				 |					 |							|							|						|						|						|
			InputClass			MainGame			         SoundClass					FPSClass				CPUClass				TimerClass				D3DClass  ShaderManagerClass---------------------------------->ColorShaderClass	   TextureShaderClass    LightShaderClass      MultiTexShaderClass		FogShaderClass	...
									 |
		  ___________________________|__________________________________________________________________________
		  |				|		        |                  |						         |					|
	 CameraClass    Transform	  TerrainClass		UserInterfaceClass	          LightClass			ParticleSystemClass
|															|
|													 _______|_______
													 |			   |
													 |			   |
												FontClass	FontShaderClass
													 |
													 |
												TextureClass

*/


//2D:

//To render 2D images to the screen you will need to calculate the screen X and Y coordinates.
//For DirectX the middle of the screen is 0,0. From there the left side of the screen and the bottom side of the screen go in the negative direction. 
//The right side of the screen and the top of the screen go in the positive direction. 
//As an example take a screen that is 1024x768 resolution
//So keep in mind that all your 2D rendering will need to work with these screen coordinate calculations 
//and that you will also need the size of the user's window/screen for correct placement of 2D images.


//Disabling Z buffer in DirectX 11
//
//To draw in 2D you should be disabling the Z buffer.
//When the Z buffer is turned off it will write the 2D data over top of whatever is in that pixel location.Make sure to use the painter's algorithm and draw from the back to the front to ensure you get your expected rendering output. 
//Once you are done drawing 2D graphics re-enable the Z buffer again so you can render 3D objects properly again.
//
//To turn the Z buffer on and off you will need to create a second depth stencil state the same as your 3D one except with DepthEnable set to false.
//Then just use OMSetDepthStencilState to switch between the two states to turn the Z buffer on and off.
//
//
//Dynamic Vertex Buffers
//
//Another new concept that will be introduced is dynamic vertex buffers.
//So far we have used static vertex buffers in the previous projects.
//The issue with static vertex buffers is that you can't change the data inside the buffer ever. 
//Dynamic vertex buffers on the other hand allow us to manipulate the information inside the vertex buffer each frame if we need to. 
//These buffers are much slower than static vertex buffers but that is the trade off for the extra functionality.
//
//The reason we use dynamic vertex buffers with 2D rendering is because we often want to move the 2D image around the screen to different locations.
//A good example is a mouse pointer, it gets moved often so the vertex data that represents its position on the screen needs to change often as well.
//
//Two extra things to note.
//Don't use dynamic vertex buffers unless they are absolutely called for, they are quite a bit slower than static buffers. 
//Secondly never destroy and recreate a static vertex buffer each frame, this can completely lock the video card 
//(which I have seen on ATI but not on Nvidia) and is far worse in overall performance 
//when compared to using dynamic vertex buffers.

//Blend:
//blendColor = basePixel * colorPixel * gammaCorrection;

//BumpNormal
//bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);

//Fog:
//Linear fog adds a linear amount of fog based on the distance you are viewing the object from:

//Linear Fog = (FogEnd - ViewpointDistance) / (FogEnd - FogStart)
//Exponential fog adds exponentially more fog the further away an object is inside the fog :
//
//Exponential Fog = 1.0 / 2.71828 power(ViewpointDistance * FogDensity)
//Exponential 2 fog adds even more exponential fog than the previous equation giving a very thick fog appearance :
//
//Exponential Fog 2 = 1.0 / 2.71828 power((ViewpointDistance * FogDensity) * (ViewpointDistance * FogDensity))
//All three equations produce a fog factor.To apply that fog factor to the model's texture and produce a final pixel color value we use the following equation:
//
//Fog Color = FogFactor * TextureColor + (1.0 - FogFactor) * FogColor
//For this tutorial we will generate the fog factor in the vertex shader and then calculate the final fog color in the pixel shader.