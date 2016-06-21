#include "Vector2D.h"

#include "Math.h"

#include "Serialization/SerializationModule.h"

#include <assert.h>

Vector2D::Vector2D()
	: ISerializable()
	, m_X(0.0f)
	, m_Y(0.0f)
{
}

Vector2D::Vector2D(float x, float y)
	: ISerializable()
	, m_X(x)
	, m_Y(y)
{
}

Vector2D::~Vector2D()
{
}

bool Vector2D::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_X, Vector2D);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Y, Vector2D);
	assert(success);

	return success;
}

Vector2D Vector2D::operator+(const Vector2D& other) const
{
	Vector2D newVector(m_X + other.m_X, m_Y + other.m_Y);
	return newVector;
}

const Vector2D& Vector2D::operator+=(const Vector2D& other)
{
	m_X += other.m_X;
	m_Y += other.m_Y;

	return *this;
}

Vector2D Vector2D::operator-(const Vector2D& other) const
{
	Vector2D newVector(m_X - other.m_X, m_Y - other.m_Y);
	return newVector;
}

const Vector2D& Vector2D::operator-=(const Vector2D& other)
{
	m_X -= other.m_X;
	m_Y -= other.m_Y;

	return *this;
}

Vector2D Vector2D::operator*(const Vector2D& other) const
{
	Vector2D newVector(m_X * other.m_X, m_Y * other.m_Y);
	return newVector;
}

const Vector2D& Vector2D::operator*=(const Vector2D& other)
{
	m_X *= other.m_X;
	m_Y *= other.m_Y;

	return *this;
}

Vector2D Vector2D::operator*(float scale) const
{
	Vector2D newVector(m_X * scale, m_Y * scale);
	return newVector;
}

const Vector2D& Vector2D::operator*=(float scale)
{
	m_X *= scale;
	m_Y *= scale;

	return *this;
}

Vector2D Vector2D::operator/(float scale) const
{
	Vector2D newVector(m_X / scale, m_Y / scale);
	return newVector;
}

const Vector2D& Vector2D::operator/=(float scale)
{
	m_X /= scale;
	m_Y /= scale;

	return *this;
}

Vector2D Vector2D::operator-() const
{
	Vector2D newVector(-m_X, -m_Y);
	return newVector;
}

void Vector2D::Rotate(float angleInDegrees)
{
	float angleInRadians = Math::DegreesToRadian(angleInDegrees);

	float cosinus = cosf(angleInRadians);
	float sinus = sinf(angleInRadians);

	float x = m_X;
	float y = m_Y;
	m_X = x * cosinus - y * sinus;
	m_Y = x * sinus + y * cosinus;
}

float Vector2D::Dot(const Vector2D& otherVector)
{
	return m_X * otherVector.m_X + m_Y * otherVector.m_Y;
}
