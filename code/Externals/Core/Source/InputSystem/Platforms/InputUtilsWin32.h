#ifndef CORE_INPUT_UTILS_WIN32_H
#define CORE_INPUT_UTILS_WIN32_H

#ifdef WIN32

#include "InputSystem/InputUtils.h"

#undef UNICODE
#include <windows.h>

namespace InputUtils
{
	Type ConvertToInputType(UINT msg);
	Key ConvertToInputKey(WPARAM wParam);
}

#endif // WIN32

#endif // CORE_INPUT_UTILS_WIN32_H