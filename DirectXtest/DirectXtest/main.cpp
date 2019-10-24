///mainFile
///メインファイル
///WindowsMain
//このファイルは、メインプログラムセッション(MainSession)です
////////////////////
//FileName:main.cpp
////////////////////

#include "systemclass.h"


///WINAPI:WindowsOSアプリケーションインターフェース。WINAPIによって、__stdcallの呼び出し規則を実装します
///__stdcallについいて：https://docs.microsoft.com/ja-jp/cpp/cpp/stdcall?view=vs-2019
///						https://docs.microsoft.com/ja-jp/cpp/cpp/argument-passing-and-naming-conventions?view=vs-2019
///中国語:https://www.cnblogs.com/yejianyong/p/7506465.html
///英語:https://docs.microsoft.com/en-us/cpp/cpp/stdcall?view=vs-2019

///HINSTANCE:Handle Instance  、クラスのインスタンスに対する"Handle"TypeのIDです、
///WindosOSが管理されるメモリーアドレスシステムです
///https://www.wdic.org/w/TECH/HINSTANCE
///中:https://baike.baidu.com/item/HINSTANCE/5123010?fr=aladdin
///英:https://en.wikipedia.org/wiki/Message_loop_in_Microsoft_Windows


//この関数はWindowsMessageLoopモデルで、"Program"をWindosシステムに生成する
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