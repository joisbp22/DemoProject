#ifndef CORE_I_EVENT_LISTENER_H
#define CORE_I_EVENT_LISTENER_H

#include <memory>

class EventBase;

class IEventListener
{
public:
	virtual ~IEventListener();

	virtual void OnEventCallback(std::weak_ptr<const EventBase> eventBase) = 0;

protected:
	IEventListener();
};

#endif // CORE_I_EVENT_LISTENER_H
