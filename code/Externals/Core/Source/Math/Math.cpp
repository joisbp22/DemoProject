#include "math.h"

#include <assert.h>

const float Math::k_PI = 3.1415926536f;

Math::Math()
{
	assert(false);
}

Math::~Math()
{
	assert(false);
}

float Math::DegreesToRadian(float angleInDegrees)
{
	return angleInDegrees / 180.0f * k_PI;
}

float Math::Min(float value1, float value2)
{
	return (value1 > value2 ? value2 : value1);
}

float Math::Max(float value1, float value2)
{
	return (value1 > value2 ? value1 : value2);
}
