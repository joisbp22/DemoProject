#include "GameModuleManager.h"

#include "GameComponentRegistrationModule.h"

#include "GameObjectSystem/GameObjectModule.h"

std::weak_ptr<GameModuleManager> GameModuleManager::s_GameModuleManager = ModuleManager::ConstructSingleton<GameModuleManager>();

GameModuleManager::GameModuleManager()
	: ModuleManager()
{
	REGISTER_FACTORY_CONSTRUCTABLE(GameComponentRegistrationModule, m_ModuleFactory);
}

GameModuleManager::~GameModuleManager()
{

}

bool GameModuleManager::Initialize()
{
	bool success = ModuleManager::Initialize();

	std::shared_ptr<GameObjectModule> gameObjectModule = GET_MODULE(GameObjectModule).lock();
	if (gameObjectModule != nullptr)
	{
		success &= gameObjectModule->LoadScene("Data/MainScene.xml");
	}

	return success;
}
