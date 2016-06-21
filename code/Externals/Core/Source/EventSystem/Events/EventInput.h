#ifndef CORE_EVENT_INPUT_H
#define CORE_EVENT_INPUT_H

#include "EventSystem/EventBase.h"

#include "InputSystem/InputUtils.h"

class EventInput : public EventBase
{
	DECLARE_EVENT_TYPE(EventInput, EventBase)
public:
	EventInput();
	~EventInput();

	void Initialize(InputUtils::Type type, InputUtils::Key key);

	InputUtils::Type GetInputType() const;
	InputUtils::Key GetInputKey() const;

private:
	InputUtils::Type m_InputType;
	InputUtils::Key m_InputKey;
};

#endif // CORE_EVENT_INPUT_H
