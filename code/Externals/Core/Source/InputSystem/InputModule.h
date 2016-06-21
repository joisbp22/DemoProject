#ifndef CORE_INPUT_MODULE_H
#define CORE_INPUT_MODULE_H

#include "ModuleSystem/ModuleBase.h"

#include "EventSystem/IEventListener.h"

#include "InputUtils.h"

class InputModule : public ModuleBase, public IEventListener
{
	DECLARE_MODULE(InputModule, ModuleBase)

public:

					InputModule();
	virtual			~InputModule() override;

	virtual bool	Initialize() override;
	virtual void	Shutdown() override;

	virtual void	OnEventCallback(std::weak_ptr<const EventBase> eventBase) override;

	bool			IsKeyPressed(InputUtils::Key key);

private:

	std::vector<bool> m_keyPressed;
};

#endif // CORE_INPUT_MODULE_H
