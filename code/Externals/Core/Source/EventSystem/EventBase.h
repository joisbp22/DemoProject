#ifndef CORE_EVENT_BASE_H
#define CORE_EVENT_BASE_H

#include "Utils/Factory.h"

class EventBase
{
	DECLARE_FACTORY_INTERFACE(EventBase)
public:
	virtual ~EventBase();

protected:
	EventBase();

};

#define DECLARE_EVENT_TYPE(ClassName, BaseClass) \
	DECLARE_FACTORY_CONSTRUCTABLE(ClassName, BaseClass, EventBase) \
	private:

#define IMPLEMENT_EVENT_TYPE(ClassName) \
	IMPLEMENT_FACTORY_CONSTRUCTABLE(ClassName, EventBase)

#endif // CORE_EVENT_BASE_H
