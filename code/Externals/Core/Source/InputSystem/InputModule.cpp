#include "InputModule.h"

#include "EventSystem/EventModule.h"
#include "EventSystem/Events/EventInput.h"

IMPLEMENT_MODULE(InputModule)


InputModule::InputModule()
{

}

InputModule::~InputModule()
{

}

bool InputModule::Initialize()
{
	bool success = true;

	m_keyPressed.resize((size_t)InputUtils::Key::eMax, false);

	REGISTER_EVENT_LISTENER(EventInput, std::dynamic_pointer_cast<IEventListener>(m_This.lock()));

	return success;
}

void InputModule::Shutdown()
{
	UNREGISTER_EVENT_LISTENER(EventInput, std::dynamic_pointer_cast<IEventListener>(m_This.lock()));
}

void InputModule::OnEventCallback(std::weak_ptr<const EventBase> eventBase)
{
	std::shared_ptr<const EventBase> eventBaseShared = eventBase.lock();
	if (eventBaseShared != nullptr && eventBaseShared->IsTypeOf(EventInput::GetType()))
	{
		std::shared_ptr<const EventInput> eventInput = std::static_pointer_cast<const EventInput>(eventBaseShared);

		switch (eventInput->GetInputType())
		{
		case InputUtils::Type::ePressed:
			if (m_keyPressed[(unsigned int)eventInput->GetInputKey()] == false)
			{
				m_keyPressed[(unsigned int)eventInput->GetInputKey()] = true;
			}
			break;
		case InputUtils::Type::eReleased:
			if (m_keyPressed[(unsigned int)eventInput->GetInputKey()])
			{
				m_keyPressed[(unsigned int)eventInput->GetInputKey()] = false;
			}
			break;
		}
	}
}

bool InputModule::IsKeyPressed(InputUtils::Key key)
{
	return m_keyPressed[(unsigned int)key];
}

