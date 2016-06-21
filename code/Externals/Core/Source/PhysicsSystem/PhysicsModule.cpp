#include "PhysicsModule.h"

#include "ICollideable.h"

#include <algorithm>

IMPLEMENT_MODULE(PhysicsModule)

PhysicsModule::PhysicsModule()
{

}

PhysicsModule::~PhysicsModule()
{

}

void PhysicsModule::Update(unsigned long deltaTime)
{
	for (auto& it : m_AABBVector)
	{
		it.Refresh();
	}

	std::sort(m_AABBVector.begin(), m_AABBVector.end(), AABB::SortX);

	std::vector<CollidingPair> collidingPairs;

	for (unsigned int i = 0; i < m_AABBVector.size(); ++i)
	{
		std::vector<AABB> xCollidingAABBVector;

		const AABB& AABB1 = m_AABBVector[i];
		for (unsigned int j = i + 1; j < m_AABBVector.size(); ++j)
		{
			const AABB& AABB2 = m_AABBVector[j];
			bool isCollidingOnXAxis = AABB1.TestCollisionX(AABB2);
			if (isCollidingOnXAxis == true)
			{
				xCollidingAABBVector.push_back(AABB2);
			}
			else
			{
				break;
			}
		}

		if (xCollidingAABBVector.empty() == false)
		{
			for (unsigned int j = 0; j < xCollidingAABBVector.size(); ++j)
			{
				const AABB& AABB2 = xCollidingAABBVector[j];
				bool isCollidingOnYAxis = AABB1.TestCollisionY(AABB2);
				if (isCollidingOnYAxis == true)
				{
					Vector2D normal;
					float displacement = 0.0f;
					bool isColliding = AABB1.TestFullCollision(AABB2, normal, displacement);
					if (isColliding)
					{
						CollidingPair newPair;

						newPair.m_Collideable1 = AABB1.GetCollideable();
						newPair.m_Collideable2 = AABB2.GetCollideable();

						newPair.m_Normal = normal;
						newPair.m_Displacement = displacement;

						collidingPairs.push_back(newPair);
					}
				}
			}
		}
	}

	for (auto it : collidingPairs)
	{
		std::shared_ptr<ICollideable> collideable1 = it.m_Collideable1.lock();
		std::shared_ptr<ICollideable> collideable2 = it.m_Collideable2.lock();
		if (collideable1 != nullptr && collideable2 != nullptr)
		{
			collideable1->CollideWith(collideable2, -it.m_Normal, it.m_Displacement);
			collideable2->CollideWith(collideable1, it.m_Normal, it.m_Displacement);
		}
	}
}

void PhysicsModule::RegisterCollideable(std::weak_ptr<ICollideable> collideable)
{
	UnregisterCollideable(collideable);
	m_AABBVector.push_back(AABB(collideable));
}

void PhysicsModule::UnregisterCollideable(std::weak_ptr<ICollideable> collideable)
{
	std::shared_ptr<ICollideable> collideableShared = collideable.lock();

	std::vector<AABB>::iterator it = m_AABBVector.begin();
	for (; it != m_AABBVector.end();)
	{
		if (it->GetCollideable().lock() == collideableShared)
		{
			it = m_AABBVector.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}
}