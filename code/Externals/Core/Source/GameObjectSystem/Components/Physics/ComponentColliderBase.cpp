#include "ComponentColliderBase.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"

#include "PhysicsSystem/PhysicsModule.h"

IMPLEMENT_COMPONENT(ComponentColliderBase);


ComponentColliderBase::ComponentColliderBase()
{

}

ComponentColliderBase::~ComponentColliderBase()
{

}

bool ComponentColliderBase::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	return false;
}

void ComponentColliderBase::Initialize()
{
	m_TransformComponent = GET_COMPONENT(ComponentTransform2D);
	RegisterCollideable(std::static_pointer_cast<ComponentColliderBase>(m_This.lock()));
}

void ComponentColliderBase::Shutdown()
{
	UnregisterCollideable(std::static_pointer_cast<ComponentColliderBase>(m_This.lock()));
}

void ComponentColliderBase::OnSetActive(bool isActive)
{
	if (isActive)
	{
		RegisterCollideable(std::static_pointer_cast<ComponentColliderBase>(m_This.lock()));
	}
	else
	{
		UnregisterCollideable(std::static_pointer_cast<ComponentColliderBase>(m_This.lock()));
	}
}

std::weak_ptr<const ComponentTransform2D> ComponentColliderBase::GetTransformComponent() const
{
	return m_TransformComponent;
}

const Vector2D& ComponentColliderBase::GetOffsetLocalSpace() const
{
	return m_OffsetLocalSpace;
}

const Point2D& ComponentColliderBase::GetCenterWorldSpace() const
{
	return m_CenterWorldSpace;
}

void ComponentColliderBase::RecalculateAABB()
{

}

bool ComponentColliderBase::TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const
{
	return false;
}

void ComponentColliderBase::CollideWith(std::weak_ptr<const ICollideable> other, const Vector2D& normal, float displacement) const
{
	std::shared_ptr<GameObject> owner = GetOwner().lock();
	if (owner != nullptr)
	{
		owner->OnCollisionEvent(std::dynamic_pointer_cast<const ComponentBase>(other.lock()), normal, displacement);
	}
}
