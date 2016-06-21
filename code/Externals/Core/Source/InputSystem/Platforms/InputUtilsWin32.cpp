#include "InputUtilsWin32.h"

#ifdef WIN32

namespace InputUtils
{
	Type ConvertToInputType(UINT msg)
	{
		Type type = Type::eNone;

		switch (msg)
		{
		case WM_KEYDOWN:
			type = Type::ePressed;
			break;
		case WM_KEYUP:
			type = Type::eReleased;
			break;
		}

		return type;
	}

	Key ConvertToInputKey(WPARAM wParam)
	{
		Key key = Key::eNone;

		switch (wParam)
		{
		case VK_LEFT:
			key = Key::eLeft;
			break;
		case VK_UP:
			key = Key::eUp;
			break;
		case VK_RIGHT:
			key = Key::eRight;
			break;
		case VK_DOWN:
			key = Key::eDown;
			break;
		case VK_SPACE:
			key = Key::eAction;
			break;
		case VK_ESCAPE:
			key = Key::eBack;
			break;
		}

		return key;
	}
}

#endif // WIN32
