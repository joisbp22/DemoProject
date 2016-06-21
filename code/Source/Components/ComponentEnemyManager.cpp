#include "ComponentEnemyManager.h"

#include "ComponentEnemy.h"

#include "GameObjectSystem/GameObjectModule.h"
#include "GameObjectSystem/GameObject.h"

#include "Serialization/SerializationModule.h"

#include "GameObjectSystem/Components/Physics/ComponentColliderBox.h"

IMPLEMENT_COMPONENT(ComponentEnemyManager)

ComponentEnemyManager::ComponentEnemyManager()
	: m_PoolSize(0)
{

}

ComponentEnemyManager::~ComponentEnemyManager()
{

}

bool ComponentEnemyManager::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_EnemyPrefab, ComponentEnemyManager);
	assert(success);

	success &= DESERIALIZE_VARIABLE(m_PoolSize, ComponentEnemyManager);
	assert(success);

	return success;
}

void ComponentEnemyManager::Initialize()
{
	std::shared_ptr<GameObjectModule> gameObjectModule = GET_MODULE(GameObjectModule).lock();
	if (gameObjectModule != nullptr)
	{
		for (unsigned int i = 0; i < m_PoolSize; ++i)
		{
			std::shared_ptr<GameObject> newEnemy = gameObjectModule->CloneObject(m_EnemyPrefab).lock();
			if (newEnemy != nullptr)
			{
				std::shared_ptr<ComponentEnemy> enemyComponent = newEnemy->GetComponent<ComponentEnemy>().lock();
				if (enemyComponent != nullptr)
				{
					newEnemy->SetActive(false);
					newEnemy->SetParent(GetOwner());

					enemyComponent->SetEnemyManager(std::static_pointer_cast<ComponentEnemyManager>(m_This.lock()));

					m_EnemyPool.push_back(enemyComponent);
				}
				else
				{
					newEnemy->Destroy();
					newEnemy.reset();
				}
			}
		}
	}
}

std::weak_ptr<ComponentEnemy> ComponentEnemyManager::SpawnEnemy()
{
	if (m_EnemyPool.empty())
		return std::weak_ptr<ComponentEnemy>();

	std::shared_ptr<ComponentEnemy> spawnedEnemy = m_EnemyPool.front().lock();
	m_EnemyPool.pop_front();

	if (spawnedEnemy != nullptr)
	{
		std::shared_ptr<GameObject> projectileOwner = spawnedEnemy->GetOwner().lock();
		if (projectileOwner != nullptr)
		{
			projectileOwner->SetActive(true);
		}
	}

	return spawnedEnemy;
}

void ComponentEnemyManager::RemoveEnemy(std::weak_ptr<ComponentEnemy> enemyToRemove)
{
	std::shared_ptr<ComponentEnemy> enemyToRemoveShared = enemyToRemove.lock();
	if (enemyToRemoveShared != nullptr)
	{
		for (auto it : m_EnemyPool)
		{
			if (it.lock() == enemyToRemoveShared)
			{
				// enemy already back in the pool
				return;
			}
		}
		std::shared_ptr<GameObject> enemyOwner = enemyToRemoveShared->GetOwner().lock();
		if (enemyOwner != nullptr)
		{
			enemyOwner->SetActive(false);

			m_EnemyPool.push_front(enemyToRemove);
		}
	}
}

unsigned int ComponentEnemyManager::GetSpawnedEnemiesCount() const
{
	return m_PoolSize - m_EnemyPool.size();
}

Vector2D ComponentEnemyManager::GetDefaultEnemyColliderHalfExtent() const
{
	std::shared_ptr<const GameObject> enemyPrefab = m_EnemyPrefab.lock();
	if (enemyPrefab != nullptr)
	{
		std::shared_ptr<const ComponentColliderBox> enemyColliderBox = enemyPrefab->GetComponent<ComponentColliderBox>().lock();
		if (enemyColliderBox != nullptr)
		{
			return enemyColliderBox->GetHalfExtentsLocalSpace();
		}
	}

	return Vector2D();
}
