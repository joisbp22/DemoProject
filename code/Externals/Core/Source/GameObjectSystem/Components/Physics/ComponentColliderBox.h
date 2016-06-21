#ifndef CORE_COMPONENT_COLLIDER_BOX_H
#define CORE_COMPONENT_COLLIDER_BOX_H

#include "ComponentColliderBase.h"

class ComponentColliderLine;

class ComponentColliderBox : public ComponentColliderBase
{
	DECLARE_COMPONENT(ComponentColliderBox, ComponentBase)

public:
	ComponentColliderBox();
	virtual ~ComponentColliderBox() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Update(unsigned long deltaTime) override;

	const Vector2D& GetHalfExtentsLocalSpace() const;
	const Vector2D& GetHalfExtentsWorldSpace() const;

protected:

	virtual void RecalculateAABB();

	virtual bool TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const override;

private:

	bool TestBoxBoxCollision(std::weak_ptr<const ComponentColliderBox> other, Vector2D& normal, float& displacement) const;
	bool TestLineBoxCollision(std::weak_ptr<const ComponentColliderLine> other, Vector2D& normal, float& displacement) const;

	Vector2D m_HalfExtentsLocalSpace;
	Vector2D m_HalfExtentsWorldSpace;
};

#endif // CORE_COMPONENT_COLLIDER_BOX_H
