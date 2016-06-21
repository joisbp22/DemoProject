#ifndef CORE_MATH_H
#define CORE_MATH_H

class Math
{
public:

	static const float k_PI;

	static float DegreesToRadian(float angleInDegrees);

	static float Min(float value1, float value2);
	static float Max(float value1, float value2);

private:
	Math();
	~Math();
};

#endif // CORE_MATH_H