#ifndef CORE_COMPONENT_RIGIDBODY_H
#define CORE_COMPONENT_RIGIDBODY_H

#include "GameObjectSystem/ComponentBase.h"

#include "Math/Vector2D.h"

class ComponentTransform2D;

class ComponentRigidBody : public ComponentBase
{
	DECLARE_COMPONENT(ComponentRigidBody, ComponentBase)

public:
	ComponentRigidBody();
	virtual ~ComponentRigidBody() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;

	virtual void Update(unsigned long deltaTime) override;

	virtual void OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement) override;

	void SetVelocity(const Vector2D& newVelocity);

private:

	std::weak_ptr<ComponentTransform2D> m_TransformComponent;

	Vector2D m_CurrentVelocity;
};

#endif // CORE_COMPONENT_RIGIDBODY_H