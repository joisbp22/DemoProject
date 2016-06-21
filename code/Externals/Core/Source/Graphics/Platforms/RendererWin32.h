#ifndef CORE_RENDERER_WIN32_H
#define CORE_RENDERER_WIN32_H

#ifdef WIN32

#include "Graphics/RendererBase.h"

class RendererWin32 : public RendererBase
{
public:
	RendererWin32();
	virtual ~RendererWin32() override;

	virtual void Initialize() override;
	virtual void Shutdown() override;

	virtual void ClearScreen() const override;

	virtual void DrawColoredLine(const Point2D& point1, const Point2D& point2, const Color& color) const override;
	virtual void DrawColoredLine(const Point2D& point, const Vector2D& vector, const Color& color) const override;

};

#endif // WIN32

#endif // CORE_RENDERER_WIN32_H