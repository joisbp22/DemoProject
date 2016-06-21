#ifndef CORE_EVENT_MODULE_H
#define CORE_EVENT_MODULE_H

#include "ModuleSystem/ModuleBase.h"

#include "IEventListener.h"
#include "EventBase.h"

class IEventListener;

class EventModule : public ModuleBase
{
	DECLARE_MODULE(EventModule, ModuleBase)

public:

					EventModule();
	virtual			~EventModule() override;

	virtual bool	Initialize() override;
	virtual void	Shutdown() override;

	void			RegisterEventListener(std::weak_ptr<IEventListener> listener, unsigned int eventType);

	void			UnregisterEventListener(std::weak_ptr<IEventListener> listener, unsigned int eventType);

	template<class T, typename... Args>
	void			DispatchEvent(unsigned int eventType, Args&&... args) const;

private:

	typedef std::vector<std::weak_ptr<IEventListener> >						EventListenersVector;
	typedef std::unordered_map<unsigned int, EventListenersVector >			EventListenersMap;
	typedef std::unordered_map<unsigned int, std::shared_ptr<EventBase> >	EventTypesMap;

	EventListenersMap	m_EventListenersMap;

	EventTypesMap		m_EventTypesMap;

	Factory<EventBase>	m_Factory;
};

#define REGISTER_EVENT_TYPE(EventType) \
	REGISTER_FACTORY_CONSTRUCTABLE(EventType, m_Factory); \
	EventTypesMap::const_iterator it = m_EventTypesMap.find(EventType::GetType()); \
	if (it == m_EventTypesMap.end()) \
	{ \
		FactoryConstructedObject<EventBase> constructedObject = m_Factory.Construct(EventType::GetType()); \
		m_EventTypesMap.insert(std::pair<unsigned int, std::shared_ptr<EventBase> >(EventType::GetType(), constructedObject.GetConstructedObject())); \
	}

#define REGISTER_EVENT_LISTENER(EventType, listener) \
	std::weak_ptr<EventModule> eventModule(GET_MODULE(EventModule)); \
	if (eventModule.expired() == false) \
	{ \
		eventModule.lock()->RegisterEventListener(listener, EventType::GetType()); \
	}

#define UNREGISTER_EVENT_LISTENER(EventType, listener) \
	std::weak_ptr<EventModule> eventModule(GET_MODULE(EventModule)); \
	if (eventModule.expired() == false) \
	{ \
		eventModule.lock()->UnregisterEventListener(listener, EventType::GetType()); \
	}

#define DISPATCH_EVENT(EventType, ...) \
	std::weak_ptr<EventModule> eventModule(GET_MODULE(EventModule)); \
	if (eventModule.expired() == false) \
	{ \
		eventModule.lock()->DispatchEvent<EventType>(EventType::GetType(), __VA_ARGS__); \
	}

#include "EventModule.hpp"

#endif // CORE_EVENT_MODULE_H
