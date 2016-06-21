#include "ComponentRigidBody.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"

#include "Serialization/SerializationModule.h"

IMPLEMENT_COMPONENT(ComponentRigidBody)

ComponentRigidBody::ComponentRigidBody()
{

}

ComponentRigidBody::~ComponentRigidBody()
{

}

bool ComponentRigidBody::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_CurrentVelocity, ComponentRigidBody);
	assert(success);

	return success;
}

void ComponentRigidBody::Initialize()
{
	m_TransformComponent = GET_COMPONENT(ComponentTransform2D);
}

void ComponentRigidBody::Update(unsigned long deltaTime)
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		float deltaTimeInSeconds = (float)deltaTime / 1000.0f;
		transformComponent->Translate(m_CurrentVelocity * deltaTimeInSeconds);
	}
}

void ComponentRigidBody::SetVelocity(const Vector2D& newVelocity)
{
	m_CurrentVelocity = newVelocity;
}

void ComponentRigidBody::OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement)
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		transformComponent->Translate(normal * displacement);
	}

	if (m_CurrentVelocity.Dot(normal) < 0.0f)
		m_CurrentVelocity = -m_CurrentVelocity;
}
