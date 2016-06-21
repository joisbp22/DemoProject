#include "ComponentBase.h"

#include "GameObject.h"

ComponentBase::ComponentBase()
{

}

ComponentBase::~ComponentBase()
{

}

void ComponentBase::Initialize()
{
}

void ComponentBase::PostInitialize()
{
}

void ComponentBase::Shutdown()
{

}

void ComponentBase::Destroy()
{

}

void ComponentBase::Update(unsigned long deltaTime)
{

}

std::weak_ptr<GameObject> ComponentBase::GetOwner() const
{
	return m_Owner;
}

void ComponentBase::SetOwner(std::weak_ptr<GameObject> owner)
{
	m_Owner = owner;
}

void ComponentBase::OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement)
{
}

void ComponentBase::OnSetActive(bool isActive)
{

}
