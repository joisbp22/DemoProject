#ifndef CORE_EVENT_MODULE_HPP
#define CORE_EVENT_MODULE_HPP

template<class T, typename... Args>
void EventModule::DispatchEvent(unsigned int eventType, Args&&... args) const
{
	EventListenersMap::const_iterator listenersIt = m_EventListenersMap.find(eventType);
	EventTypesMap::const_iterator typesIt = m_EventTypesMap.find(eventType);
	if (listenersIt != m_EventListenersMap.end() && typesIt != m_EventTypesMap.end())
	{
		std::shared_ptr<T> tShared = std::static_pointer_cast<T>(typesIt->second);
		tShared->Initialize(std::forward<Args>(args)...);

		const EventListenersVector& eventListenersVector = listenersIt->second;
		for (const auto listenerIt : eventListenersVector)
		{
			std::shared_ptr<IEventListener> listener = listenerIt.lock();
			if (listener != nullptr)
				listener->OnEventCallback(typesIt->second);
		}
	}
}

#endif // CORE_EVENT_MODULE_HPP
