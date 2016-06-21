#include "ComponentColliderLine.h"

#include "GameObjectSystem/Components/Physics/ComponentColliderBox.h"
#include "GameObjectSystem/Components/ComponentTransform2D.h"

#include "Serialization/SerializationModule.h"

#include "Math/Math.h"
#include <cfloat>

IMPLEMENT_COMPONENT(ComponentColliderLine);


ComponentColliderLine::ComponentColliderLine()
{

}

ComponentColliderLine::~ComponentColliderLine()
{

}

bool ComponentColliderLine::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_OffsetLocalSpace, ComponentColliderLine);
	assert(success);

	success &= DESERIALIZE_VARIABLE(m_VectorLocalSpace, ComponentColliderLine);
	assert(success);

	return success;
}

void ComponentColliderLine::Update(unsigned long deltaTime)
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		const Transform2D& worldTransform = transformComponent->GetWorldTransform();

		Vector2D transformedOffset = worldTransform.LocalToWorld(m_OffsetLocalSpace);
		m_CenterWorldSpace = worldTransform.m_Position + transformedOffset;

		m_TransformedVector = worldTransform.LocalToWorld(m_VectorLocalSpace);
	}

	RecalculateAABB();
}

const Vector2D& ComponentColliderLine::GetVectorWorldSpace() const
{
	return m_TransformedVector;
}

void ComponentColliderLine::RecalculateAABB()
{
	std::shared_ptr<ComponentTransform2D> transformComponent = m_TransformComponent.lock();
	if (transformComponent != nullptr)
	{
		Point2D points[2];
		points[0] = m_CenterWorldSpace;
		points[1] = m_CenterWorldSpace + m_TransformedVector;
		m_Min = Point2D(FLT_MAX, FLT_MAX);
		m_Max = Point2D(FLT_MIN, FLT_MIN);
		for (unsigned int i = 0; i < 2; ++i)
		{
			m_Min.m_X = Math::Min(points[i].m_X, m_Min.m_X);
			m_Min.m_Y = Math::Min(points[i].m_Y, m_Min.m_Y);

			m_Max.m_X = Math::Max(points[i].m_X, m_Max.m_X);
			m_Max.m_Y = Math::Max(points[i].m_Y, m_Max.m_Y);
		}
	}
}

bool ComponentColliderLine::TestFullCollision(std::weak_ptr<const ICollideable> other, Vector2D& normal, float& displacement) const
{
	std::shared_ptr<const ComponentColliderBase> otherBaseComponent = std::static_pointer_cast<const ComponentColliderBase>(other.lock());
	if (otherBaseComponent != nullptr)
	{
		if (otherBaseComponent->IsTypeOf(ComponentColliderBox::GetType()))
		{
			std::shared_ptr<const ComponentColliderBox> otherBoxComponent = std::static_pointer_cast<const ComponentColliderBox>(otherBaseComponent);
			TestLineBoxCollision(otherBoxComponent, normal, displacement);
		}
		// todo : make a line-line collision
	}
	return false;
}

bool ComponentColliderLine::TestLineBoxCollision(std::weak_ptr<const ComponentColliderBox> other, Vector2D& normal, float& displacement) const
{
	std::shared_ptr<const ComponentColliderBox> otherBoxComponent = other.lock();
	if (otherBoxComponent != nullptr)
	{
		std::shared_ptr<const ComponentTransform2D> boxTransformComponent = otherBoxComponent->GetTransformComponent().lock();
		if (boxTransformComponent != nullptr)
		{
			const Transform2D& boxWorldTransform = boxTransformComponent->GetWorldTransform();

			const Point2D& lineCenterLocalSpace = boxWorldTransform.WorldToLocal(m_CenterWorldSpace);
			const Vector2D& lineVectorLocalSpace = boxWorldTransform.WorldToLocal(m_TransformedVector);

			const Vector2D& boxOffsetLocalSpace = otherBoxComponent->GetOffsetLocalSpace();
			const Vector2D& boxHalfExtentsLocalSpace = otherBoxComponent->GetHalfExtentsLocalSpace();

			Point2D boxColliderPoints[4];
			boxColliderPoints[0] = Point2D() + boxOffsetLocalSpace + Vector2D(boxHalfExtentsLocalSpace.m_X, boxHalfExtentsLocalSpace.m_Y);
			boxColliderPoints[1] = Point2D() + boxOffsetLocalSpace + Vector2D(boxHalfExtentsLocalSpace.m_X, -boxHalfExtentsLocalSpace.m_Y);
			boxColliderPoints[2] = Point2D() + boxOffsetLocalSpace + Vector2D(-boxHalfExtentsLocalSpace.m_X, boxHalfExtentsLocalSpace.m_Y);
			boxColliderPoints[3] = Point2D() + boxOffsetLocalSpace + Vector2D(-boxHalfExtentsLocalSpace.m_X, -boxHalfExtentsLocalSpace.m_Y);

			Point2D lineColliderPoints[2];
			lineColliderPoints[0] = lineCenterLocalSpace;
			lineColliderPoints[1] = lineCenterLocalSpace + lineVectorLocalSpace;

			Point2D min(FLT_MAX, FLT_MAX);
			Point2D max(FLT_MIN, FLT_MIN);
			for (unsigned int i = 0; i < 4; ++i)
			{
				min.m_X = Math::Min(boxColliderPoints[i].m_X, min.m_X);
				min.m_Y = Math::Min(boxColliderPoints[i].m_Y, min.m_Y);

				max.m_X = Math::Max(boxColliderPoints[i].m_X, max.m_X);
				max.m_Y = Math::Max(boxColliderPoints[i].m_Y, max.m_Y);
			}

			bool isIntersecting = false;
			for (unsigned int i = 0; i < 2; ++i)
			{
				const Point2D& linePoint = lineColliderPoints[i];
				if (linePoint.m_X > min.m_X && linePoint.m_X < max.m_X && linePoint.m_Y > min.m_Y && linePoint.m_Y < max.m_Y)
				{
					float minXDifference = linePoint.m_X - min.m_X;
					float maxXDifference = max.m_X - linePoint.m_X;
					float minYDifference = linePoint.m_Y - min.m_Y;
					float maxYDifference = max.m_Y - linePoint.m_Y;
					if (minXDifference <= maxXDifference && minXDifference <= minYDifference && minXDifference <= maxYDifference)
					{
						displacement = minXDifference;
						normal = boxWorldTransform.m_Matrix.GetRightVector();
					}
					else if (maxXDifference <= minXDifference && maxXDifference <= minYDifference && maxXDifference <= maxYDifference)
					{
						displacement = maxXDifference;
						normal = -boxWorldTransform.m_Matrix.GetRightVector();
					}
					else if (minYDifference <= minXDifference && minYDifference <= maxXDifference && minYDifference <= maxYDifference)
					{
						displacement = minYDifference;
						normal = boxWorldTransform.m_Matrix.GetUpVector();
					}
					else if (maxYDifference <= minXDifference && maxYDifference <= maxXDifference && maxYDifference <= minYDifference)
					{
						displacement = maxYDifference;
						normal = -boxWorldTransform.m_Matrix.GetUpVector();
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
