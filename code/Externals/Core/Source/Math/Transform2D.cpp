#include "Transform2D.h"

#include "Serialization/SerializationModule.h"

const Vector2D Transform2D::k_UpDefault = Vector2D(0.0f, 1.0f);
const Vector2D Transform2D::k_RightDefault = Vector2D(1.0f, 0.0f);

Transform2D::Transform2D()
	: m_Scale(1.0f)
{

}

Transform2D::~Transform2D()
{

}

bool Transform2D::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_Position, Transform2D);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Scale, Transform2D);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Matrix, Transform2D);
	assert(success);

	return success;
}

Transform2D	Transform2D::WorldToLocal(const Transform2D& worldTransform) const
{
	Transform2D newTransform(*this);
	newTransform.m_Position = WorldToLocal(worldTransform.m_Position);
	newTransform.m_Matrix.Inverse();
	newTransform.m_Matrix *= worldTransform.m_Matrix;
	newTransform.m_Scale = worldTransform.m_Scale / newTransform.m_Scale;
	return newTransform;
}

Transform2D	Transform2D::LocalToWorld(const Transform2D& localTransform) const
{
	Transform2D newTransform(*this);
	newTransform.m_Position = LocalToWorld(localTransform.m_Position);
	newTransform.m_Matrix = localTransform.m_Matrix * newTransform.m_Matrix;
	newTransform.m_Scale *= localTransform.m_Scale;
	return newTransform;
}

Vector2D Transform2D::WorldToLocal(const Vector2D& worldVector) const
{
	Matrix2D matrixCopy(m_Matrix);
	matrixCopy.Inverse();
	Vector2D newVector = matrixCopy * worldVector;
	newVector /= m_Scale;
	return newVector;
}

Vector2D Transform2D::LocalToWorld(const Vector2D& localVector) const
{
	Vector2D newVector = m_Matrix * localVector;
	newVector *= m_Scale;
	return newVector;
}

Point2D Transform2D::WorldToLocal(const Point2D& worldPoint) const
{
	Vector2D localVector = WorldToLocal(worldPoint - m_Position);
	return Point2D() + localVector;
}

Point2D Transform2D::LocalToWorld(const Point2D& localPoint) const
{
	Vector2D worldVector = LocalToWorld(localPoint - Point2D());
	return m_Position + worldVector;
}
