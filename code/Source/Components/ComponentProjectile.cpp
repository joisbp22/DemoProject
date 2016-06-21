#include "ComponentProjectile.h"

#include "ComponentProjectileManager.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"
#include "GameObjectSystem/Components/Physics/ComponentRigidBody.h"

#include "Serialization/SerializationModule.h"

#include "Application/ApplicationBase.h"

IMPLEMENT_COMPONENT(ComponentProjectile)

ComponentProjectile::ComponentProjectile()
	: m_Speed(0.0f)
{

}

ComponentProjectile::~ComponentProjectile()
{

}

bool ComponentProjectile::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_Speed, ComponentProjectile);
	assert(success);

	return success;
}

void ComponentProjectile::Initialize()
{
	m_TransformComponent = GET_COMPONENT(ComponentTransform2D);
	m_RigidBodyComponent = GET_COMPONENT(ComponentRigidBody);
}

void ComponentProjectile::Update(unsigned long deltaTime)
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		const Transform2D& worldTransform = transformComponent->GetWorldTransform();

		std::shared_ptr<ApplicationBase> application = ApplicationBase::GetInstance<ApplicationBase>().lock();
		if (application != nullptr)
		{
			const ApplicationSettings& applicationSettings = application->GetSettings();
			float screenWidth = (float)applicationSettings.GetScreenWidth();
			float screenHeight = (float)applicationSettings.GetScreenHeight();

			if (worldTransform.m_Position.m_X < -100.0f
				|| worldTransform.m_Position.m_X > screenWidth + 100.0f
				|| worldTransform.m_Position.m_Y < -100.0f
				|| worldTransform.m_Position.m_Y > screenHeight + 100.0f)
			{
				// we're out of bounds -> despawn ourselves
				std::shared_ptr<ComponentProjectileManager> projectileManager = m_ProjectileManager.lock();
				if (projectileManager != nullptr)
				{
					projectileManager->RemoveProjectile(std::static_pointer_cast<ComponentProjectile>(m_This.lock()));
				}
			}
		}
	}
}

void ComponentProjectile::OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement)
{
	std::shared_ptr<const ComponentBase> otherShared = other.lock();
	if (otherShared != nullptr)
	{
		std::shared_ptr<const GameObject> otherOwner = otherShared->GetOwner().lock();
		if (otherOwner != nullptr)
		{
			// let's make sure we didn't hit another projectile
			std::weak_ptr<ComponentProjectile> otherComponentProjectile = otherOwner->GetComponent<ComponentProjectile>();
			if (otherComponentProjectile.expired())
			{
				std::shared_ptr<ComponentProjectileManager> projectileManager = m_ProjectileManager.lock();
				if (projectileManager != nullptr)
				{
					projectileManager->RemoveProjectile(std::static_pointer_cast<ComponentProjectile>(m_This.lock()));
				}
			}
		}
	}
}

void ComponentProjectile::SetProjectileManager(std::weak_ptr<ComponentProjectileManager> projectileManager)
{
	m_ProjectileManager = projectileManager;
}

void ComponentProjectile::Fire(const Point2D& worldSartPosition, const Matrix2D& worldRotation)
{
	std::shared_ptr<ComponentTransform2D> projectileTransformComponent = m_TransformComponent.lock();
	if (projectileTransformComponent != nullptr)
	{
		projectileTransformComponent->SetWorldPosition(worldSartPosition);
		projectileTransformComponent->SetWorldRotation(worldRotation);
	}

	std::shared_ptr<ComponentRigidBody> projectileRigidBodyComponent = m_RigidBodyComponent.lock();
	if (projectileRigidBodyComponent != nullptr)
	{
		Vector2D direction = worldRotation * Transform2D::k_UpDefault;
		projectileRigidBodyComponent->SetVelocity(direction * m_Speed);
	}
}
