#include "AABB.h"

#include "ICollideable.h"
#include <assert.h>

AABB::AABB()
{

}

AABB::AABB(std::weak_ptr<ICollideable> collideable)
	: m_Collideable(collideable)
{
}

AABB::~AABB()
{

}

void AABB::Refresh()
{
	std::shared_ptr<ICollideable> collideableShared = m_Collideable.lock();
	assert(collideableShared != nullptr);
	if (collideableShared != nullptr)
	{
		m_Min = collideableShared->GetMin();
		m_Max = collideableShared->GetMax();
	}
}

std::weak_ptr<ICollideable> AABB::GetCollideable() const
{
	return m_Collideable;
}

bool AABB::TestCollisionX(const AABB& other) const
{
	return (m_Min.m_X < other.m_Max.m_X) && (m_Max.m_X > other.m_Min.m_X);
}

bool AABB::TestCollisionY(const AABB& other) const
{
	return (m_Min.m_Y < other.m_Max.m_Y) && (m_Max.m_Y > other.m_Min.m_Y);
}

bool AABB::TestFullCollision(const AABB& other, Vector2D& normal, float& displacement) const
{
	std::shared_ptr<ICollideable> collideableShared1 = m_Collideable.lock();
	std::shared_ptr<ICollideable> collideableShared2 = other.m_Collideable.lock();

	bool hit = false;
	hit |= collideableShared1->TestFullCollision(collideableShared2, normal, displacement);
	if (hit)
	{
		return true;
	}

	hit |= collideableShared2->TestFullCollision(collideableShared1, normal, displacement);
	normal = -normal;

	return hit;
	//std::shared_ptr<ICollideable> collideableShared1 = m_Collideable.lock();
	//std::shared_ptr<ICollideable> collideableShared2 = other.m_Collideable.lock();

	//return collideableShared1->TestFullCollision(collideableShared2, normal, displacement);
}

bool AABB::SortX(const AABB& aabb1, const AABB& aabb2)
{
	return aabb1.m_Min.m_X < aabb2.m_Min.m_X;
}

bool AABB::SortY(const AABB& aabb1, const AABB& aabb2)
{
	return aabb1.m_Min.m_Y < aabb2.m_Min.m_Y;
}