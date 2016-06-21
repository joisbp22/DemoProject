#ifndef CORE_TRANSFORM_2D_H
#define CORE_TRANSFORM_2D_H

#include "Point2D.h"
#include "Matrix2D.h"

class Transform2D : public ISerializable
{
public:
					Transform2D();
	virtual			~Transform2D() override;

	virtual bool	Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	Transform2D		WorldToLocal(const Transform2D& worldTransform) const;
	Transform2D		LocalToWorld(const Transform2D& localTransform) const;

	Vector2D		WorldToLocal(const Vector2D& worldVector) const;
	Vector2D		LocalToWorld(const Vector2D& localVector) const;

	Point2D			WorldToLocal(const Point2D& worldVector) const;
	Point2D			LocalToWorld(const Point2D& localVector) const;

	Point2D		m_Position;
	float		m_Scale;
	Matrix2D	m_Matrix;

	static const Vector2D k_UpDefault;
	static const Vector2D k_RightDefault;
};

#endif // CORE_TRANSFORM_2D_H
