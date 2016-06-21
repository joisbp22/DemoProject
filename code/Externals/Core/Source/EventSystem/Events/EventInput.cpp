#include "EventInput.h"

IMPLEMENT_EVENT_TYPE(EventInput)

EventInput::EventInput()
	: EventBase()
	, m_InputType(InputUtils::Type::eNone)
	, m_InputKey(InputUtils::Key::eNone)
{

}

EventInput::~EventInput()
{

}

void EventInput::Initialize(InputUtils::Type type, InputUtils::Key key)
{
	m_InputType = type;
	m_InputKey = key;
}

InputUtils::Type EventInput::GetInputType() const
{
	return m_InputType;
}

InputUtils::Key EventInput::GetInputKey() const
{
	return m_InputKey;
}
