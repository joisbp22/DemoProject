#ifndef GAME_COMPONENT_ENEMY_MANAGER_H
#define GAME_COMPONENT_ENEMY_MANAGER_H

#include "GameObjectSystem/ComponentBase.h"

#include <list>
#include "Math/Vector2D.h"

class ComponentEnemy;

class ComponentEnemyManager : public ComponentBase
{
	DECLARE_COMPONENT(ComponentEnemyManager, ComponentBase)
public:
	ComponentEnemyManager();
	virtual ~ComponentEnemyManager() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	std::weak_ptr<ComponentEnemy> SpawnEnemy();
	void RemoveEnemy(std::weak_ptr<ComponentEnemy> enemyToRemove);

	unsigned int GetSpawnedEnemiesCount() const;
	Vector2D GetDefaultEnemyColliderHalfExtent() const;

private:
	std::weak_ptr<GameObject> m_EnemyPrefab;
	unsigned int m_PoolSize;

	std::list<std::weak_ptr<ComponentEnemy>> m_EnemyPool;
};

#endif // GAME_COMPONENT_ENEMY_MANAGER_H