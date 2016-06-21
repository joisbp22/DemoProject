#ifndef CORE_AABB_H
#define CORE_AABB_H

#include <memory>
#include "Math/Point2D.h"

class ICollideable;

class AABB
{
public:
	AABB();
	AABB(std::weak_ptr<ICollideable> collideable);
	~AABB();

	void Refresh();

	std::weak_ptr<ICollideable> GetCollideable() const;

	bool TestCollisionX(const AABB& other) const;
	bool TestCollisionY(const AABB& other) const;
	bool TestFullCollision(const AABB& other, Vector2D& normal, float& displacement) const;

	static bool SortX(const AABB& aabb1, const AABB& aabb2);
	static bool SortY(const AABB& aabb1, const AABB& aabb2);

private:

	Point2D m_Min;
	Point2D m_Max;

	std::weak_ptr<ICollideable> m_Collideable;
};

#endif // CORE_AABB_H