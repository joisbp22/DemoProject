#include "ComponentEnemy.h"

#include "ComponentEnemyManager.h"
#include "ComponentProjectile.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"
#include "GameObjectSystem/Components/Physics/ComponentRigidBody.h"

#include "Serialization/SerializationModule.h"

#include "Application/ApplicationBase.h"

IMPLEMENT_COMPONENT(ComponentEnemy)

ComponentEnemy::ComponentEnemy()
	: m_Speed(0.0f)
	, m_CurrentScaleLevel(0)
{

}

ComponentEnemy::~ComponentEnemy()
{

}

bool ComponentEnemy::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_Speed, ComponentEnemy);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_ScaleLevels, ComponentEnemy);
	assert(success);

	return success;
}

void ComponentEnemy::Initialize()
{
	m_TransformComponent = GET_COMPONENT(ComponentTransform2D);
	m_RigidBodyComponent = GET_COMPONENT(ComponentRigidBody);

	SetScaleLevel(0);
}

void ComponentEnemy::Update(unsigned long deltaTime)
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
				std::shared_ptr<ComponentEnemyManager> enemyManager = m_EnemyManager.lock();
				if (enemyManager != nullptr)
				{
					SetScaleLevel(0);
					enemyManager->RemoveEnemy(std::static_pointer_cast<ComponentEnemy>(m_This.lock()));
				}
			}
		}
	}
}

void ComponentEnemy::OnCollisionEvent(std::weak_ptr<const ComponentBase> other, const Vector2D& normal, float displacement)
{
	std::shared_ptr<const ComponentBase> otherShared = other.lock();
	if (otherShared != nullptr)
	{
		std::shared_ptr<const GameObject> otherOwner = otherShared->GetOwner().lock();
		if (otherOwner != nullptr)
		{
			std::weak_ptr<ComponentProjectile> otherComponentProjectile = otherOwner->GetComponent<ComponentProjectile>().lock();
			if (otherComponentProjectile.expired() == false)
			{
				// we hit a projectile
				if (m_CurrentScaleLevel < m_ScaleLevels.size())
				{
					float newScale = m_ScaleLevels[m_CurrentScaleLevel];

					std::shared_ptr<ComponentEnemyManager> enemyManager = m_EnemyManager.lock();
					if (enemyManager != nullptr)
					{
						std::shared_ptr<ComponentTransform2D> enemyTransformComponent = m_TransformComponent.lock();
						if (enemyTransformComponent != nullptr)
						{
							const Transform2D& localTransform = enemyTransformComponent->GetLocalTransform();
							Point2D currentPosition = localTransform.m_Position;

							enemyTransformComponent->SetLocalScale(newScale);
							Vector2D defaultHalfExtents = enemyManager->GetDefaultEnemyColliderHalfExtent();
							defaultHalfExtents *= newScale;
							enemyTransformComponent->Translate(defaultHalfExtents);
							enemyTransformComponent->UpdateWorldTransform();

							std::shared_ptr<GameObject> thisOwner = GetOwner().lock();
							if (thisOwner != nullptr)
							{
								thisOwner->Update(0);
							}

							std::shared_ptr<ComponentEnemy> enemyComponent = enemyManager->SpawnEnemy().lock();
							if (enemyComponent != nullptr)
							{
								enemyComponent->Spawn(currentPosition - defaultHalfExtents);
								enemyComponent->SetScaleLevel(m_CurrentScaleLevel);

								std::shared_ptr<GameObject> newEnemyOwner = enemyComponent->GetOwner().lock();
								if (newEnemyOwner != nullptr)
								{
									newEnemyOwner->Update(0);
								}
							}
						}
					}

					++m_CurrentScaleLevel;
				}
				else
				{
					std::shared_ptr<ComponentEnemyManager> enemyManager = m_EnemyManager.lock();
					if (enemyManager != nullptr)
					{
						SetScaleLevel(0);
						enemyManager->RemoveEnemy(std::static_pointer_cast<ComponentEnemy>(m_This.lock()));
					}
				}
			}
		}
	}
}

void ComponentEnemy::SetEnemyManager(std::weak_ptr<ComponentEnemyManager> enemyManager)
{
	m_EnemyManager = enemyManager;
}

void ComponentEnemy::Spawn(const Point2D& worldPosition)
{
	std::shared_ptr<ComponentTransform2D> enemyTransformComponent = m_TransformComponent.lock();
	if (enemyTransformComponent != nullptr)
	{
		enemyTransformComponent->SetWorldPosition(worldPosition);
		enemyTransformComponent->UpdateWorldTransform();
	}

	std::shared_ptr<ComponentRigidBody> enemyRigidBodyComponent = m_RigidBodyComponent.lock();
	if (enemyRigidBodyComponent != nullptr)
	{
		Vector2D direction = Transform2D::k_UpDefault;
		direction.Rotate((float)(rand() % 360));
		enemyRigidBodyComponent->SetVelocity(direction * m_Speed);
	}
}

void ComponentEnemy::SetScaleLevel(unsigned int scaleLevel)
{
	if (scaleLevel < m_ScaleLevels.size())
	{
		float newScale = m_ScaleLevels[scaleLevel];
		m_CurrentScaleLevel = scaleLevel + 1;

		std::shared_ptr<ComponentTransform2D> enemyTransformComponent = m_TransformComponent.lock();
		if (enemyTransformComponent != nullptr)
		{
			const Transform2D& localTransform = enemyTransformComponent->GetLocalTransform();
			Point2D currentPosition = localTransform.m_Position;

			enemyTransformComponent->SetLocalScale(newScale);
			enemyTransformComponent->UpdateWorldTransform();
		}
	}
}
