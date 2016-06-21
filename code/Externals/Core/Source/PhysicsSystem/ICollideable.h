#ifndef CORE_ICOLLIDEABLE_H
#define CORE_ICOLLIDEABLE_H

#include <memory>
#include "Math/Point2D.h"

class PhysicsModule;

class ICollideable
{
public:
	ICollideable();
	virtual ~ICollideable();

	static void RegisterCollideable(std::weak_ptr<ICollideable> collideable);
	static void UnregisterCollideable(std::weak_ptr<ICollideable> collideable);

	const Point2D& GetMin() const;
	const Point2D& GetMax() const;

	virtual bool TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const = 0;
	virtual void CollideWith(std::weak_ptr<const ICollideable> other, const Vector2D& normal, float displacement) const = 0;

protected:

	virtual void RecalculateAABB() = 0;

	Point2D m_Min;
	Point2D m_Max;
};

#endif // CORE_ICOLLIDEABLE_H
