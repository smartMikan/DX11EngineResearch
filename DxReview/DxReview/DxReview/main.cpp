#include "./System/systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Create the system object.
	//�V�X�e���I�u�W�F�N�g�𐶐�����
	System = new SystemClass;
	if (!System)
	{
		//���s����ꍇ�A�I�����āA���s����0��߂�
		return 0;
	}

	// Initialize and run the system object.
	//������
	result = System->Initialize();
	if (result)
	{
		//��������ꍇ�ARun()�֐������s
		System->Run();
	}


	//�I������
	// Shutdown and release the system object.
	//�I�u�W�F�N�g�����A�������[�����[�X�֐�
	System->ProgramOver();
	delete System;
	System = 0;

	return 0;
}