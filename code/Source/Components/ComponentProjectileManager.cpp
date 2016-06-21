#include "ComponentProjectileManager.h"

#include "ComponentProjectile.h"

#include "GameObjectSystem/GameObjectModule.h"
#include "GameObjectSystem/GameObject.h"

#include "Serialization/SerializationModule.h"

IMPLEMENT_COMPONENT(ComponentProjectileManager)

ComponentProjectileManager::ComponentProjectileManager()
	: m_PoolSize(0)
{

}

ComponentProjectileManager::~ComponentProjectileManager()
{

}

bool ComponentProjectileManager::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_ProjectilePrefab, ComponentProjectileManager);
	assert(success);

	success &= DESERIALIZE_VARIABLE(m_PoolSize, ComponentProjectileManager);
	assert(success);

	return success;
}

void ComponentProjectileManager::Initialize()
{
	std::shared_ptr<GameObjectModule> gameObjectModule = GET_MODULE(GameObjectModule).lock();
	if (gameObjectModule != nullptr)
	{
		for (unsigned int i = 0; i < m_PoolSize; ++i)
		{
			std::shared_ptr<GameObject> newProjectile = gameObjectModule->CloneObject(m_ProjectilePrefab).lock();
			if (newProjectile != nullptr)
			{
				std::shared_ptr<ComponentProjectile> projectileComponent = newProjectile->GetComponent<ComponentProjectile>().lock();
				if (projectileComponent != nullptr)
				{
					newProjectile->SetActive(false);
					newProjectile->SetParent(GetOwner());

					projectileComponent->SetProjectileManager(std::static_pointer_cast<ComponentProjectileManager>(m_This.lock()));

					m_ProjectilePool.push_back(projectileComponent);
				}
				else
				{
					newProjectile->Destroy();
					newProjectile.reset();
				}
			}
		}
	}
}

std::weak_ptr<ComponentProjectile> ComponentProjectileManager::SpawnProjectile()
{
	if (m_ProjectilePool.empty())
		return std::weak_ptr<ComponentProjectile>();

	std::shared_ptr<ComponentProjectile> spawnedProjectile = m_ProjectilePool.front().lock();
	m_ProjectilePool.pop_front();

	if (spawnedProjectile != nullptr)
	{
		std::shared_ptr<GameObject> projectileOwner = spawnedProjectile->GetOwner().lock();
		if (projectileOwner != nullptr)
		{
			projectileOwner->SetActive(true);
		}
	}

	return spawnedProjectile;
}

void ComponentProjectileManager::RemoveProjectile(std::weak_ptr<ComponentProjectile> projectileToRemove)
{
	std::shared_ptr<ComponentProjectile> projectileToRemoveShared = projectileToRemove.lock();
	if (projectileToRemoveShared != nullptr)
	{
		for (auto it : m_ProjectilePool)
		{
			if (it.lock() == projectileToRemoveShared)
			{
				// projectile already back in the pool
				return;
			}
		}
		std::shared_ptr<GameObject> projectileOwner = projectileToRemoveShared->GetOwner().lock();
		if (projectileOwner != nullptr)
		{
			projectileOwner->SetActive(false);

			m_ProjectilePool.push_back(projectileToRemove);
		}
	}
}
