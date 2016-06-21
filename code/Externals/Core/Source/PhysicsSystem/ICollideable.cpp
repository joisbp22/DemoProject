#include "ICollideable.h"

#include "GameObjectSystem/GameObject.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"

#include "PhysicsSystem/PhysicsModule.h"

ICollideable::ICollideable()
{

}

ICollideable::~ICollideable()
{

}

void ICollideable::RegisterCollideable(std::weak_ptr<ICollideable> collideable)
{
	std::shared_ptr<PhysicsModule> physicsModule = GET_MODULE(PhysicsModule).lock();
	if (physicsModule != nullptr)
	{
		physicsModule->RegisterCollideable(collideable);
	}
}

void ICollideable::UnregisterCollideable(std::weak_ptr<ICollideable> collideable)
{
	std::shared_ptr<PhysicsModule> physicsModule = GET_MODULE(PhysicsModule).lock();
	if (physicsModule != nullptr)
	{
		physicsModule->UnregisterCollideable(collideable);
	}
}

const Point2D& ICollideable::GetMin() const
{
	return m_Min;
}

const Point2D& ICollideable::GetMax() const
{
	return m_Max;
}
