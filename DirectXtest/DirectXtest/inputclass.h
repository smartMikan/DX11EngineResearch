#pragma once
///
//To keep the tutorials simple I used the windows input for the time being until I do a tutorial on DirectInput (which is far superior). 
//The input class handles the user input from the keyboard. 
//This class is given input from the SystemClass::MessageHandler function.
//The input object will store the state of each key in a keyboard array. 
//When queried it will tell the calling functions if a certain key is pressed.



//////////////////////////
// Filename: inputclass.h
//////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

//You need to define the version of Direct Input you are using in the header or the compiler will generate annoying messages that it is defaulting to version 8.
///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


//The following two libraries need to be linked for Direct Input to work.

/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

//////////////////////////
// Class name: InputClass
//////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();
	void GetMouseLocation(int&, int&);


	/*void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);*/

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;
	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	//
	//bool m_keys[256];	//Down:True //Up:False
};

#endif

