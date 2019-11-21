#include "./System/systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Create the system object.
	//システムオブジェクトを生成する
	System = new SystemClass;
	if (!System)
	{
		//失敗する場合、終了して、実行結果0を戻す
		return 0;
	}

	// Initialize and run the system object.
	//初期化
	result = System->Initialize();
	if (result)
	{
		//成功する場合、Run()関数を実行
		System->Run();
	}


	//終了処理
	// Shutdown and release the system object.
	//オブジェクト消す、メモリーリリース関数
	System->ProgramOver();
	delete System;
	System = 0;

	return 0;
}