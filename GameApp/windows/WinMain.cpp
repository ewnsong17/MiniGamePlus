#include "stdafx.h"
#include "system/SystemClass.h"

INT APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszcmParam, int cmdShow)
{
	SystemClass* system = new SystemClass;

	if (system)
	{
		if (system->Initialize())
		{
			system->Run();
		}

		system->Shutdown();
		delete system;
		system = nullptr;

		return 0;
	}
	return -1;
}