#include "Matrix2D.h"

#include "Math.h"

#include "Serialization/SerializationModule.h"

#include <assert.h>

Matrix2D::Matrix2D()
	: ISerializable()
{
	m_Components[0] = 1.0f;
	m_Components[1] = 0.0f;
	m_Components[2] = 0.0f;
	m_Components[3] = 1.0f;
}

Matrix2D::Matrix2D(float component00, float component01, float component10, float component11)
	: ISerializable()
{
	m_Components[0] = component00;
	m_Components[1] = component01;
	m_Components[2] = component10;
	m_Components[3] = component11;
}

Matrix2D::~Matrix2D()
{
}

bool Matrix2D::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE_ARRAY(m_Components, Matrix2D);
	assert(success);

	return success;
}

Matrix2D Matrix2D::operator*(const Matrix2D& other) const
{
	Matrix2D newMatrix(
		m_Components[0] * other.m_Components[0] + m_Components[1] * other.m_Components[2],
		m_Components[0] * other.m_Components[1] + m_Components[1] * other.m_Components[3],
		m_Components[2] * other.m_Components[0] + m_Components[3] * other.m_Components[2],
		m_Components[2] * other.m_Components[1] + m_Components[3] * other.m_Components[3]
		);
	return newMatrix;
}

const Matrix2D& Matrix2D::operator*=(const Matrix2D& other)
{
	Matrix2D newMatrix(
		m_Components[0] * other.m_Components[0] + m_Components[1] * other.m_Components[2],
		m_Components[0] * other.m_Components[1] + m_Components[1] * other.m_Components[3],
		m_Components[2] * other.m_Components[0] + m_Components[3] * other.m_Components[2],
		m_Components[2] * other.m_Components[1] + m_Components[3] * other.m_Components[3]
		);

	*this = newMatrix;

	return *this;
}

Vector2D Matrix2D::operator*(const Vector2D& other) const
{
	Vector2D newVector(
		m_Components[0] * other.m_X + m_Components[1] * other.m_Y,
		m_Components[2] * other.m_X + m_Components[3] * other.m_Y
		);
	return newVector;
}

const Matrix2D& Matrix2D::SetRotation(float angleInDegrees)
{
	float angleInRadians = Math::DegreesToRadian(angleInDegrees);

	float cosinus = cosf(angleInRadians);
	float sinus = sinf(angleInRadians);

	Matrix2D newMatrix(
		cosinus,
		-sinus,
		sinus,
		cosinus
		);

	*this = newMatrix;

	return *this;
}

const Matrix2D& Matrix2D::Rotate(float angleInDegrees)
{
	float angleInRadians = Math::DegreesToRadian(angleInDegrees);

	float cosinus = cosf(angleInRadians);
	float sinus = sinf(angleInRadians);

	Matrix2D newMatrix(
		cosinus,
		-sinus,
		sinus,
		cosinus
		);

	*this *= newMatrix;

	return *this;
}

const Matrix2D& Matrix2D::Inverse()
{
	float det = m_Components[0] * m_Components[3] - m_Components[1] * m_Components[2];

	Matrix2D newMatrix(
		m_Components[3] / det,
		-m_Components[1] / det,
		-m_Components[2] / det,
		m_Components[0] / det
		);

	*this = newMatrix;

	return *this;
}

Vector2D Matrix2D::GetUpVector() const
{
	return Vector2D(m_Components[1], m_Components[3]);
}

Vector2D Matrix2D::GetRightVector() const
{
	return Vector2D(m_Components[0], m_Components[2]);
}
