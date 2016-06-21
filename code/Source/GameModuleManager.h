#ifndef GAME_MODULE_MANAGER_H
#define GAME_MODULE_MANAGER_H

#include "ModuleSystem/ModuleManager.h"

class GameModuleManager : public ModuleManager
{
public:
	GameModuleManager();
	virtual ~GameModuleManager() override;

	virtual bool Initialize() override;

private:
	static std::weak_ptr<GameModuleManager> GameModuleManager::s_GameModuleManager;
};

#endif // GAME_MODULE_MANAGER_H