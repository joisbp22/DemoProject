#ifndef GAME_COMPONENT_PROJECTILE_MANAGER_H
#define GAME_COMPONENT_PROJECTILE_MANAGER_H

#include "GameObjectSystem/ComponentBase.h"

#include <list>

class ComponentProjectile;

class ComponentProjectileManager : public ComponentBase
{
	DECLARE_COMPONENT(ComponentProjectileManager, ComponentBase)
public:
	ComponentProjectileManager();
	virtual ~ComponentProjectileManager() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	std::weak_ptr<ComponentProjectile> SpawnProjectile();
	void RemoveProjectile(std::weak_ptr<ComponentProjectile> projectileToRemove);

private:
	std::weak_ptr<GameObject> m_ProjectilePrefab;
	unsigned int m_PoolSize;

	std::list<std::weak_ptr<ComponentProjectile>> m_ProjectilePool;
};

#endif // GAME_COMPONENT_PROJECTILE_MANAGER_H