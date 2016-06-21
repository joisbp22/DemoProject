#include "ComponentColliderBox.h"

#include "GameObjectSystem/Components/Physics/ComponentColliderLine.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"
#include "GameObjectSystem/GameObject.h"

#include "Serialization/SerializationModule.h"

#include "Math/Math.h"
#include <cfloat>

IMPLEMENT_COMPONENT(ComponentColliderBox);


ComponentColliderBox::ComponentColliderBox()
{

}

ComponentColliderBox::~ComponentColliderBox()
{

}

bool ComponentColliderBox::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_OffsetLocalSpace, ComponentColliderBox);
	assert(success);

	success &= DESERIALIZE_VARIABLE(m_HalfExtentsLocalSpace, ComponentColliderBox);
	assert(success);

	return success;
}

void ComponentColliderBox::Update(unsigned long deltaTime)
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		const Transform2D& worldTransform = transformComponent->GetWorldTransform();

		Vector2D transformedOffset = worldTransform.LocalToWorld(m_OffsetLocalSpace);
		m_CenterWorldSpace = worldTransform.m_Position + transformedOffset;

		m_HalfExtentsWorldSpace = worldTransform.LocalToWorld(m_HalfExtentsLocalSpace);
	}
	RecalculateAABB();
}

const Vector2D& ComponentColliderBox::GetHalfExtentsLocalSpace() const
{
	return m_HalfExtentsLocalSpace;
}

const Vector2D& ComponentColliderBox::GetHalfExtentsWorldSpace() const
{
	return m_HalfExtentsWorldSpace;
}

void ComponentColliderBox::RecalculateAABB()
{
	Point2D points[4];
	points[0] = m_CenterWorldSpace + Vector2D(m_HalfExtentsWorldSpace.m_X, m_HalfExtentsWorldSpace.m_Y);
	points[1] = m_CenterWorldSpace + Vector2D(m_HalfExtentsWorldSpace.m_X, -m_HalfExtentsWorldSpace.m_Y);
	points[2] = m_CenterWorldSpace + Vector2D(-m_HalfExtentsWorldSpace.m_X, m_HalfExtentsWorldSpace.m_Y);
	points[3] = m_CenterWorldSpace + Vector2D(-m_HalfExtentsWorldSpace.m_X, -m_HalfExtentsWorldSpace.m_Y);

	m_Min = Point2D(FLT_MAX, FLT_MAX);
	m_Max = Point2D(FLT_MIN, FLT_MIN);
	for (unsigned int i = 0; i < 4; ++i)
	{
		m_Min.m_X = Math::Min(points[i].m_X, m_Min.m_X);
		m_Min.m_Y = Math::Min(points[i].m_Y, m_Min.m_Y);

		m_Max.m_X = Math::Max(points[i].m_X, m_Max.m_X);
		m_Max.m_Y = Math::Max(points[i].m_Y, m_Max.m_Y);
	}
}

bool ComponentColliderBox::TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const
{
	std::shared_ptr<const ComponentColliderBase> otherBaseComponent = std::dynamic_pointer_cast<const ComponentColliderBase>(other.lock());
	if (otherBaseComponent != nullptr)
	{
		if (otherBaseComponent->IsTypeOf(ComponentColliderBox::GetType()))
		{
			std::shared_ptr<const ComponentColliderBox> otherBoxComponent = std::static_pointer_cast<const ComponentColliderBox>(otherBaseComponent);
			return TestBoxBoxCollision(otherBoxComponent, normal, displacement);
		}
		else if (otherBaseComponent->IsTypeOf(ComponentColliderLine::GetType()))
		{
			std::shared_ptr<const ComponentColliderLine> otherLineComponent = std::static_pointer_cast<const ComponentColliderLine>(otherBaseComponent);
			return TestLineBoxCollision(otherLineComponent, normal, displacement);
		}
	}
	return false;
}

bool ComponentColliderBox::TestBoxBoxCollision(std::weak_ptr<const ComponentColliderBox> other, Vector2D& normal, float& displacement) const
{
	std::shared_ptr<const ComponentColliderBox> otherBoxComponent = other.lock();
	if (otherBoxComponent != nullptr)
	{
		std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
		if (transformComponent != nullptr)
		{
			const Transform2D& worldTransform = transformComponent->GetWorldTransform();

			const Point2D& otherCenterLocalSpace = worldTransform.WorldToLocal(otherBoxComponent->GetCenterWorldSpace());
			const Vector2D& otherHalfExtentsLocalSpace = worldTransform.WorldToLocal(otherBoxComponent->GetHalfExtentsWorldSpace());

			Point2D thisColliderPoints[4];
			thisColliderPoints[0] = Point2D() + m_OffsetLocalSpace + Vector2D(m_HalfExtentsLocalSpace.m_X, m_HalfExtentsLocalSpace.m_Y);
			thisColliderPoints[1] = Point2D() + m_OffsetLocalSpace + Vector2D(m_HalfExtentsLocalSpace.m_X, -m_HalfExtentsLocalSpace.m_Y);
			thisColliderPoints[2] = Point2D() + m_OffsetLocalSpace + Vector2D(-m_HalfExtentsLocalSpace.m_X, m_HalfExtentsLocalSpace.m_Y);
			thisColliderPoints[3] = Point2D() + m_OffsetLocalSpace + Vector2D(-m_HalfExtentsLocalSpace.m_X, -m_HalfExtentsLocalSpace.m_Y);

			Point2D otherColliderPoints[4];
			otherColliderPoints[0] = otherCenterLocalSpace + Vector2D(otherHalfExtentsLocalSpace.m_X, otherHalfExtentsLocalSpace.m_Y);
			otherColliderPoints[1] = otherCenterLocalSpace + Vector2D(otherHalfExtentsLocalSpace.m_X, -otherHalfExtentsLocalSpace.m_Y);
			otherColliderPoints[2] = otherCenterLocalSpace + Vector2D(-otherHalfExtentsLocalSpace.m_X, otherHalfExtentsLocalSpace.m_Y);
			otherColliderPoints[3] = otherCenterLocalSpace + Vector2D(-otherHalfExtentsLocalSpace.m_X, -otherHalfExtentsLocalSpace.m_Y);

			Point2D min (FLT_MAX, FLT_MAX);
			Point2D max (FLT_MIN, FLT_MIN);
			for (unsigned int i = 0; i < 4; ++i)
			{
				min.m_X = Math::Min(thisColliderPoints[i].m_X, min.m_X);
				min.m_Y = Math::Min(thisColliderPoints[i].m_Y, min.m_Y);

				max.m_X = Math::Max(thisColliderPoints[i].m_X, max.m_X);
				max.m_Y = Math::Max(thisColliderPoints[i].m_Y, max.m_Y);
			}

			bool isIntersecting = false;
			for (unsigned int i = 0; i < 4; ++i)
			{
				const Point2D& otherPoint = otherColliderPoints[i];
				if (otherPoint.m_X > min.m_X && otherPoint.m_X < max.m_X && otherPoint.m_Y > min.m_Y && otherPoint.m_Y < max.m_Y)
				{
					float minXDifference = otherPoint.m_X - min.m_X;
					float maxXDifference = max.m_X - otherPoint.m_X;
					float minYDifference = otherPoint.m_Y - min.m_Y;
					float maxYDifference = max.m_Y - otherPoint.m_Y;
					if (minXDifference <= maxXDifference && minXDifference <= minYDifference && minXDifference <= maxYDifference)
					{
						displacement = minXDifference;
						normal = -worldTransform.m_Matrix.GetRightVector();
					}
					else if (maxXDifference <= minXDifference && maxXDifference <= minYDifference && maxXDifference <= maxYDifference)
					{
						displacement = maxXDifference;
						normal = worldTransform.m_Matrix.GetRightVector();
					}
					else if (minYDifference <= minXDifference && minYDifference <= maxXDifference && minYDifference <= maxYDifference)
					{
						displacement = minYDifference;
						normal = -worldTransform.m_Matrix.GetUpVector();
					}
					else if (maxYDifference <= minXDifference && maxYDifference <= maxXDifference && maxYDifference <= minYDifference)
					{
						displacement = maxYDifference;
						normal = worldTransform.m_Matrix.GetUpVector();
					}

					isIntersecting = true;
					break;
				}
			}

			return isIntersecting;
		}
	}

	return false;
}

bool ComponentColliderBox::TestLineBoxCollision(std::weak_ptr<const ComponentColliderLine> other, Vector2D& normal, float& displacement) const
{
	std::shared_ptr<const ComponentColliderLine> otherLineComponent = other.lock();
	if (otherLineComponent != nullptr)
	{
		std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
		if (transformComponent != nullptr)
		{
			const Transform2D& worldTransform = transformComponent->GetWorldTransform();

			const Point2D& otherCenterLocalSpace = worldTransform.WorldToLocal(otherLineComponent->GetCenterWorldSpace());
			const Vector2D& otherVectorLocalSpace = worldTransform.WorldToLocal(otherLineComponent->GetVectorWorldSpace());

			Point2D thisColliderPoints[4];
			thisColliderPoints[0] = Point2D() + m_OffsetLocalSpace + Vector2D(m_HalfExtentsLocalSpace.m_X, m_HalfExtentsLocalSpace.m_Y);
			thisColliderPoints[1] = Point2D() + m_OffsetLocalSpace + Vector2D(m_HalfExtentsLocalSpace.m_X, -m_HalfExtentsLocalSpace.m_Y);
			thisColliderPoints[2] = Point2D() + m_OffsetLocalSpace + Vector2D(-m_HalfExtentsLocalSpace.m_X, m_HalfExtentsLocalSpace.m_Y);
			thisColliderPoints[3] = Point2D() + m_OffsetLocalSpace + Vector2D(-m_HalfExtentsLocalSpace.m_X, -m_HalfExtentsLocalSpace.m_Y);

			Point2D otherColliderPoints[2];
			otherColliderPoints[0] = otherCenterLocalSpace;
			otherColliderPoints[1] = otherCenterLocalSpace + otherVectorLocalSpace;

			Point2D min(FLT_MAX, FLT_MAX);
			Point2D max(FLT_MIN, FLT_MIN);
			for (unsigned int i = 0; i < 4; ++i)
			{
				min.m_X = Math::Min(thisColliderPoints[i].m_X, min.m_X);
				min.m_Y = Math::Min(thisColliderPoints[i].m_Y, min.m_Y);

				max.m_X = Math::Max(thisColliderPoints[i].m_X, max.m_X);
				max.m_Y = Math::Max(thisColliderPoints[i].m_Y, max.m_Y);
			}

			bool isIntersecting = false;
			for (unsigned int i = 0; i < 2; ++i)
			{
				const Point2D& otherPoint = otherColliderPoints[i];
				if (otherPoint.m_X > min.m_X && otherPoint.m_X < max.m_X && otherPoint.m_Y > min.m_Y && otherPoint.m_Y < max.m_Y)
				{
					float minXDifference = otherPoint.m_X - min.m_X;
					float maxXDifference = max.m_X - otherPoint.m_X;
					float minYDifference = otherPoint.m_Y - min.m_Y;
					float maxYDifference = max.m_Y - otherPoint.m_Y;
					if (minXDifference <= maxXDifference && minXDifference <= minYDifference && minXDifference <= maxYDifference)
					{
						displacement = minXDifference;
						normal = -worldTransform.m_Matrix.GetRightVector();
					}
					else if (maxXDifference <= minXDifference && maxXDifference <= minYDifference && maxXDifference <= maxYDifference)
					{
						displacement = maxXDifference;
						normal = worldTransform.m_Matrix.GetRightVector();
					}
					else if (minYDifference <= minXDifference && minYDifference <= maxXDifference && minYDifference <= maxYDifference)
					{
						displacement = minYDifference;
						normal = -worldTransform.m_Matrix.GetUpVector();
					}
					else if (maxYDifference <= minXDifference && maxYDifference <= maxXDifference && maxYDifference <= minYDifference)
					{
						displacement = maxYDifference;
						normal = worldTransform.m_Matrix.GetUpVector();
					}

					isIntersecting = true;
					break;
				}
			}

			return isIntersecting;
		}
	}

	return false;
}
