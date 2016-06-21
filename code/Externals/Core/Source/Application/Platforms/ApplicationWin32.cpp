#include "ApplicationWin32.h"

#ifdef WIN32

#include <assert.h>

#include "InputSystem/Platforms/InputUtilsWin32.h"
#include "EventSystem/EventModule.h"
#include "EventSystem/Events/EventInput.h"


ApplicationWin32::ApplicationWin32()
	: ApplicationBase()
{

}


ApplicationWin32::~ApplicationWin32()
{

}

bool ApplicationWin32::Initialize()
{
	bool success = ApplicationBase::Initialize();

	return success;
}

void ApplicationWin32::Shutdown()
{
	ApplicationBase::Shutdown();
}

void ApplicationWin32::Run(HINSTANCE hInstance)
{
	m_HInstance = hInstance;

	// Initialize application
	bool success = Initialize();
	assert(success == true);
	if (success == false)
	{
		return;
	}

	// Main game loop
	while (true)
	{
		// Pause sleepTimeMs milliseconds
		DWORD lastTime = timeGetTime();

		const DWORD sleepTimeMs = 1000 / GetSettings().GetFramerateMax();
		const DWORD currentTime = timeGetTime();
		const DWORD timeSinceLast = currentTime - lastTime;

		DWORD deltaTime = timeSinceLast;
		if (timeSinceLast < sleepTimeMs)
		{
			Sleep(sleepTimeMs - timeSinceLast);
			deltaTime = sleepTimeMs;
		}

		lastTime = currentTime;

		// Windows messaging
		// *** do not remove ***
		MSG msg;
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Update application
		Update(deltaTime);
	}

	Shutdown();
}

HINSTANCE ApplicationWin32::GetHInstance() const
{
	return m_HInstance;
}

void ApplicationWin32::Update(unsigned long deltaTime)
{
	ApplicationBase::Update(deltaTime);
}

LRESULT CALLBACK ApplicationWin32::WindowsEventCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::shared_ptr<ApplicationWin32> applicationWin32 = ApplicationWin32::GetInstance<ApplicationWin32>().lock();
	assert(applicationWin32 != nullptr);
	if (applicationWin32 != nullptr)
	{
		return applicationWin32->WindowsEventCallbackInternal(hwnd, msg, wParam, lParam);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT ApplicationWin32::WindowsEventCallbackInternal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		break;
		case WM_PAINT:
		{
		}
		break;
		case WM_KEYDOWN:
		{
			// todo : do this through event system
			switch (wParam)
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
			}

			InputUtils::Key keyPressed = InputUtils::ConvertToInputKey(wParam);
			DISPATCH_EVENT(EventInput, InputUtils::Type::ePressed, keyPressed);
			break;
		}
		case WM_KEYUP:
		{
			InputUtils::Key keyReleased = InputUtils::ConvertToInputKey(wParam);
			DISPATCH_EVENT(EventInput, InputUtils::Type::eReleased, keyReleased);
			break;
		}
		default:
		{
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif // WIN32
