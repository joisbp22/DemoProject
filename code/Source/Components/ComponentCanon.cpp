#include "ComponentCanon.h"

#include "ComponentProjectileManager.h"
#include "ComponentProjectile.h"

#include "ModuleSystem/ModuleManager.h"
#include "InputSystem/InputModule.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"
#include "GameObjectSystem/Components/Physics/ComponentRigidBody.h"

#include "Serialization/SerializationModule.h"

IMPLEMENT_COMPONENT(ComponentCanon)

ComponentCanon::ComponentCanon()
	: m_RotationSpeed(0.0f)
	, m_MinRotationInDegrees(0.0f)
	, m_MaxRotationInDegrees(0.0f)
	, m_CurrentRotationInDegrees(0.0f)
{

}

ComponentCanon::~ComponentCanon()
{

}

bool ComponentCanon::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_ProjectileManager, ComponentCanon);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_RotationSpeed, ComponentCanon);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_MinRotationInDegrees, ComponentCanon);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_MaxRotationInDegrees, ComponentCanon);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_TimeBetweenShots, ComponentCanon);
	assert(success);

	assert(m_MaxRotationInDegrees > m_MinRotationInDegrees);

	m_CurrentRotationInDegrees = (m_MaxRotationInDegrees + m_MinRotationInDegrees) / 2.0f;

	return success;
}

void ComponentCanon::Initialize()
{
	m_InputModule = GET_MODULE(InputModule);
	m_TransformComponent = GET_COMPONENT(ComponentTransform2D);

	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		m_InitialLocalRotation = transformComponent->GetLocalTransform().m_Matrix;
	}
	ApplyRotation();
}

void ComponentCanon::Update(unsigned long deltaTime)
{
	std::shared_ptr<InputModule> inputModule = m_InputModule.lock();
	if (inputModule != nullptr)
	{
		if (inputModule->IsKeyPressed(InputUtils::Key::eLeft))
		{
			m_CurrentRotationInDegrees -= m_RotationSpeed * ((float)deltaTime / 1000.0f);
			if (m_CurrentRotationInDegrees < m_MinRotationInDegrees)
			{
				m_CurrentRotationInDegrees = m_MinRotationInDegrees;
			}
			ApplyRotation();
		}

		if (inputModule->IsKeyPressed(InputUtils::Key::eRight))
		{
			m_CurrentRotationInDegrees += m_RotationSpeed * ((float)deltaTime / 1000.0f);
			if (m_CurrentRotationInDegrees > m_MaxRotationInDegrees)
			{
				m_CurrentRotationInDegrees = m_MaxRotationInDegrees;
			}
			ApplyRotation();
		}

		if (inputModule->IsKeyPressed(InputUtils::Key::eAction))
		{
			if (m_TimeUntilNextShot <= 0.0f)
			{
				m_TimeUntilNextShot = m_TimeBetweenShots;
				std::shared_ptr<ComponentProjectileManager> projectileManager = m_ProjectileManager.lock();
				if (projectileManager != nullptr)
				{
					std::shared_ptr<ComponentProjectile> projectile = projectileManager->SpawnProjectile().lock();
					if (projectile != nullptr)
					{
						std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
						if (transformComponent != nullptr)
						{
							const Transform2D& canonWorldTransform = transformComponent->GetWorldTransform();

							projectile->Fire(canonWorldTransform.m_Position + canonWorldTransform.m_Matrix.GetUpVector() * 30, canonWorldTransform.m_Matrix);
						}
					}
				}
			}
		}
	}

	CheckFireRate(deltaTime);
}

void ComponentCanon::ApplyRotation()
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		Matrix2D newRotation(m_InitialLocalRotation);
		newRotation.Rotate(m_CurrentRotationInDegrees);

		transformComponent->SetLocalRotation(newRotation);
	}
}

void ComponentCanon::CheckFireRate(unsigned long deltaTime)
{
	if (m_TimeUntilNextShot > 0.0f)
	{
		m_TimeUntilNextShot -= ((float)deltaTime / 1000.0f);
	}
}
