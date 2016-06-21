#ifndef CORE_VECTOR2D_H
#define CORE_VECTOR2D_H

#include "Serialization/ISerializable.h"

class Vector2D : public ISerializable
{
public:
	Vector2D();
	Vector2D(float x, float y);
	virtual ~Vector2D() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	Vector2D operator+(const Vector2D& other) const;
	const Vector2D& operator+=(const Vector2D& other);

	Vector2D operator-(const Vector2D& other) const;
	const Vector2D& operator-=(const Vector2D& other);

	Vector2D operator*(const Vector2D& other) const;
	const Vector2D& operator*=(const Vector2D& other);

	Vector2D operator*(float scale) const;
	const Vector2D& operator*=(float scale);

	Vector2D operator/(float scale) const;
	const Vector2D& operator/=(float scale);

	Vector2D operator-() const;

	void Rotate(float angleInDegrees);

	float Dot(const Vector2D& otherVector);

	float m_X;
	float m_Y;
};

#endif // CORE_VECTOR2D_H