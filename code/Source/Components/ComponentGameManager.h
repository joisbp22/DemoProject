#ifndef GAME_COMPONENT_GAME_MANAGER_H
#define GAME_COMPONENT_GAME_MANAGER_H

#include "GameObjectSystem/ComponentBase.h"

class ComponentEnemyManager;

class ComponentGameManager : public ComponentBase
{
	DECLARE_COMPONENT(ComponentGameManager, ComponentBase)
public:
	ComponentGameManager();
	virtual ~ComponentGameManager() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void PostInitialize() override;

	virtual void Update(unsigned long deltaTime) override;

private:

	void StartNewGame();

	unsigned int m_InitialEnemyCount;
	std::weak_ptr<ComponentEnemyManager> m_EnemyManagerComponent;
};

#endif // GAME_COMPONENT_GAME_MANAGER_H