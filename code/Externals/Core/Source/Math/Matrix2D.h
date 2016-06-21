#ifndef CORE_MATRIX_2D_H
#define CORE_MATRIX_2D_H

#include "Vector2D.h"

class Matrix2D : public ISerializable
{
public:
	Matrix2D();
	Matrix2D(float component00, float component01, float component10, float component11);
	virtual ~Matrix2D() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	Matrix2D operator*(const Matrix2D& other) const;
	const Matrix2D& operator*=(const Matrix2D& other);

	Vector2D operator*(const Vector2D& other) const;

	const Matrix2D& SetRotation(float angleInDegrees);
	const Matrix2D& Rotate(float angleInDegrees);

	const Matrix2D& Inverse();

	Vector2D GetUpVector() const;
	Vector2D GetRightVector() const;

	float m_Components[4];
};

#endif // CORE_MATRIX_2D_H