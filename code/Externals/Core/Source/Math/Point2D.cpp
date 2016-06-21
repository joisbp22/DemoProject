#include "Point2D.h"


#include "Serialization/SerializationModule.h"

#include <assert.h>

Point2D::Point2D()
	: ISerializable()
	, m_X(0.0f)
	, m_Y(0.0f)
{
}

Point2D::Point2D(float x, float y)
	: ISerializable()
	, m_X(x)
	, m_Y(y)
{
}

Point2D::~Point2D()
{
}

bool Point2D::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_X, Point2D);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Y, Point2D);
	assert(success);

	return success;
}

Point2D Point2D::operator+(const Vector2D& other) const
{
	Point2D newPoint(m_X + other.m_X, m_Y + other.m_Y);
	return newPoint;
}

const Point2D& Point2D::operator+=(const Vector2D& other)
{
	m_X += other.m_X;
	m_Y += other.m_Y;

	return *this;
}

Vector2D Point2D::operator-(const Point2D& other) const
{
	Vector2D newVector(m_X - other.m_X, m_Y - other.m_Y);
	return newVector;
}

Point2D Point2D::operator-() const
{
	Point2D newPoint(-m_X, -m_Y);
	return newPoint;
}

Point2D Point2D::operator-(const Vector2D& other) const
{
	Point2D newPoint(m_X - other.m_X, m_Y - other.m_Y);
	return newPoint;
}

Point2D Point2D::operator-=(const Vector2D& other)
{
	m_X -= other.m_X;
	m_Y -= other.m_Y;

	return *this;
}
