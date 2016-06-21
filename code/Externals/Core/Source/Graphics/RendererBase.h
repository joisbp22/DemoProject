#ifndef CORE_RENDERER_BASE_H
#define CORE_RENDERER_BASE_H

class Point2D;
class Vector2D;
class Color;

class RendererBase
{
public:
	virtual ~RendererBase();

	virtual void Initialize();
	virtual void Shutdown();

	virtual void ClearScreen() const;

	virtual void DrawColoredLine(const Point2D& point1, const Point2D& point2, const Color& color) const;
	virtual void DrawColoredLine(const Point2D& point, const Vector2D& vector, const Color& color) const;

	unsigned int GetScreenWidth() const;
	unsigned int GetScreenHeight() const;

protected:
	RendererBase();

	unsigned int m_ScreenWidth;
	unsigned int m_ScreenHeight;
};

#endif // CORE_RENDERER_BASE_H