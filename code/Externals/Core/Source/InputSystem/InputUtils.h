#ifndef CORE_INPUT_UTILS_H
#define CORE_INPUT_UTILS_H

namespace InputUtils
{
	enum class Type
	{
		eNone = 0,
		ePressed,
		eReleased,

		eMax
	};

	enum class Key
	{
		eNone = 0,
		eLeft,
		eUp,
		eRight,
		eDown,
		eAction,
		eBack,

		eMax
	};
}

#endif // CORE_INPUT_UTILS_H