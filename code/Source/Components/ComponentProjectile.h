#ifndef GAME_COMPONENT_PROJECTILE_H
#define GAME_COMPONENT_PROJECTILE_H

#include "GameObjectSystem/ComponentBase.h"

#include "Math/Point2D.h"
#include "Math/Matrix2D.h"

class ComponentProjectileManager;
class ComponentTransform2D;
class ComponentRigidBody;

class ComponentProjectile : public ComponentBase
{
	DECLARE_COMPONENT(ComponentProjectile, ComponentBase)
public:
	ComponentProjectile();
	virtual ~ComponentProjectile() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	virtual void Update(unsigned long deltaTime);

	virtual void OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement);

	void SetProjectileManager(std::weak_ptr<ComponentProjectileManager> projectileManager);

	void Fire(const Point2D& worldSartPosition, const Matrix2D& worldRotation);

private:
	std::weak_ptr<ComponentProjectileManager> m_ProjectileManager;

	std::weak_ptr<ComponentTransform2D> m_TransformComponent;
	std::weak_ptr<ComponentRigidBody> m_RigidBodyComponent;

	float m_Speed;
};

#endif // GAME_COMPONENT_PROJECTILE_H