#ifndef CORE_POINT2D_H
#define CORE_POINT2D_H

#include "Serialization/ISerializable.h"

#include "Vector2D.h"

class Point2D : public ISerializable
{
public:
	Point2D();
	Point2D(float x, float y);
	virtual ~Point2D() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	Point2D operator+(const Vector2D& other) const;
	const Point2D& operator+=(const Vector2D& other);

	Vector2D operator-(const Point2D& other) const;
	Point2D operator-(const Vector2D& other) const;
	Point2D operator-=(const Vector2D& other);
	Point2D operator-() const;

	float m_X;
	float m_Y;
};

#endif // CORE_POINT2D_H