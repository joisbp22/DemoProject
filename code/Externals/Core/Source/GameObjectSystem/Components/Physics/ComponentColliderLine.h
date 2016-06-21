#ifndef CORE_COMPONENT_COLLIDER_LINE_H
#define CORE_COMPONENT_COLLIDER_LINE_H

#include "ComponentColliderBase.h"

class ComponentColliderBox;

class ComponentColliderLine : public ComponentColliderBase
{
	DECLARE_COMPONENT(ComponentColliderLine, ComponentBase)

public:
	ComponentColliderLine();
	virtual ~ComponentColliderLine() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Update(unsigned long deltaTime) override;

	const Vector2D& GetVectorWorldSpace() const;

protected:

	virtual void RecalculateAABB();

	virtual bool TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const override;

private:

	bool TestLineBoxCollision(std::weak_ptr<const ComponentColliderBox> other, Vector2D& normal, float& displacement) const;

	Vector2D m_VectorLocalSpace;

	Vector2D m_TransformedVector;
};

#endif // CORE_COMPONENT_COLLIDER_LINE_H
