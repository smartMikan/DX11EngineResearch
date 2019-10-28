#pragma once
//参考:http://rastertek.com/dx11s2tut02.html
//説明(Description):
/*骨組み(Framework):
							WinMain
							   |
							   |
						  SystemClass
							   |
				  _____________|_____
				 |					 |
				 |					 |
			InputClass			GraphicClass
									 |
		  ___________________________|__________________________________________________
		 |				|		        |                 |							    |
	  D3DClass      ModelClass       CameraClass      ColorShaderClass				TextureShaderClass
						|
						|
					TextureClass



*/