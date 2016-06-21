#ifndef GAME_COMPONENT_ENEMY_H
#define GAME_COMPONENT_ENEMY_H

#include "GameObjectSystem/ComponentBase.h"

#include "Math/Point2D.h"
#include "Math/Matrix2D.h"

class ComponentEnemyManager;
class ComponentTransform2D;
class ComponentRigidBody;

class ComponentEnemy : public ComponentBase
{
	DECLARE_COMPONENT(ComponentEnemy, ComponentBase)
public:
	ComponentEnemy();
	virtual ~ComponentEnemy() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	virtual void Update(unsigned long deltaTime);

	virtual void OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement);

	void SetEnemyManager(std::weak_ptr<ComponentEnemyManager> enemyManager);

	void Spawn(const Point2D& worldPosition);
	void SetScaleLevel(unsigned int scaleLevel);

private:
	std::weak_ptr<ComponentEnemyManager> m_EnemyManager;

	std::weak_ptr<ComponentTransform2D> m_TransformComponent;
	std::weak_ptr<ComponentRigidBody> m_RigidBodyComponent;

	float m_Speed;

	std::vector<float> m_ScaleLevels;
	unsigned int m_CurrentScaleLevel;
};

#endif // GAME_COMPONENT_ENEMY_H