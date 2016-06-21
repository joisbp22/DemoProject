#ifndef CORE_APPLICATION_WIN32_H
#define CORE_APPLICATION_WIN32_H

#ifdef WIN32

#include "Application/ApplicationBase.h"

#undef UNICODE
#include <windows.h>

class ApplicationWin32 : public ApplicationBase
{
public:
	
							ApplicationWin32();
	virtual					~ApplicationWin32() override;

	virtual bool			Initialize() override;
	virtual void			Shutdown() override;

	void					Run(HINSTANCE hInstance);

	HINSTANCE				GetHInstance() const;

	static LRESULT CALLBACK WindowsEventCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:

	virtual void			Update(unsigned long deltaTime);

	LRESULT					WindowsEventCallbackInternal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HINSTANCE					m_HInstance;
};

#endif // WIN32

#endif // CORE_APPLICATION_WIN32_H
