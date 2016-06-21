#ifndef CORE_COMPONENT_COLLIDER_BASE_H
#define CORE_COMPONENT_COLLIDER_BASE_H

#include "GameObjectSystem/ComponentBase.h"
#include "PhysicsSystem/ICollideable.h"

#include "Math/Vector2D.h"

class ComponentTransform2D;

class ComponentColliderBase : public ComponentBase, public ICollideable
{
	DECLARE_COMPONENT(ComponentColliderBase, ComponentBase)

public:
	ComponentColliderBase();
	virtual ~ComponentColliderBase() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;
	virtual void Shutdown() override;

	virtual void OnSetActive(bool isActive) override;

	std::weak_ptr<const ComponentTransform2D> GetTransformComponent() const;
	const Vector2D& GetOffsetLocalSpace() const;
	const Point2D& GetCenterWorldSpace() const;

protected:

	virtual void RecalculateAABB() override;

	virtual bool TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const override;
	virtual void CollideWith(std::weak_ptr<const ICollideable> other, const Vector2D& normal, float displacement) const override;

	std::weak_ptr<ComponentTransform2D> m_TransformComponent;

	Vector2D m_OffsetLocalSpace;
	Point2D m_CenterWorldSpace;
};

#endif // CORE_COMPONENT_COLLIDER_H
