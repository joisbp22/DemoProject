#ifndef GAME_COMPONENT_REGISTRATION_MODULE_H
#define GAME_COMPONENT_REGISTRATION_MODULE_H

#include "ModuleSystem/ModuleBase.h"

class GameComponentRegistrationModule : public ModuleBase
{
	DECLARE_MODULE(GameComponentRegistrationModule, ModuleBase)

public:
	GameComponentRegistrationModule();
	virtual ~GameComponentRegistrationModule() override;

	virtual void RegisterSerializables(Factory<Serializable>& factory) override;
};

#endif // GAME_TEST_H