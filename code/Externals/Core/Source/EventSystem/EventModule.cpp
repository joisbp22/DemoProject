#include "EventModule.h"

#include "Events/EventInput.h"

IMPLEMENT_MODULE(EventModule)

EventModule::EventModule()
	: ModuleBase()
{
	REGISTER_EVENT_TYPE(EventInput);
}

EventModule::~EventModule()
{

}

bool EventModule::Initialize()
{
	bool success = true;

	return success;
}

void EventModule::Shutdown()
{

}

void EventModule::RegisterEventListener(std::weak_ptr<IEventListener> listener, unsigned int eventType)
{
	UnregisterEventListener(listener, eventType);
	m_EventListenersMap[eventType].push_back(listener);
}

void EventModule::UnregisterEventListener(std::weak_ptr<IEventListener> listener, unsigned int eventType)
{
	EventListenersMap::iterator mapIt = m_EventListenersMap.find(eventType);
	if (mapIt != m_EventListenersMap.end())
	{
		EventListenersVector& eventListenersVector = mapIt->second;
		EventListenersVector::iterator it = eventListenersVector.begin();
		for (; it != eventListenersVector.end(); )
		{
			std::shared_ptr<IEventListener> registeredListenerShared = it->lock();
			std::shared_ptr<IEventListener> listenerShared = listener.lock();
			if (registeredListenerShared == listenerShared)
			{
				it = eventListenersVector.erase(it);
				return;
			}
			else
			{
				++it;
			}
		}
	}
}
