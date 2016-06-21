#ifdef WIN32

#undef UNICODE
#include <windows.h>

#include <memory>

#include "Application/Platforms/ApplicationWin32.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ApplicationWin32::ConstructSingleton<ApplicationWin32>();
	std::shared_ptr<ApplicationWin32> application = ApplicationWin32::GetInstance<ApplicationWin32>().lock();
	application->Run(hInstance);
	application.reset();
	ApplicationWin32::DestroySingleton();

	return 0;
}

#endif // WIN32