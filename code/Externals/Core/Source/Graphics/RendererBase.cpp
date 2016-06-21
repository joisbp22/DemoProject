#include "RendererBase.h"


RendererBase::RendererBase()
	: m_ScreenWidth(0)
	, m_ScreenHeight(0)
{

}

RendererBase::~RendererBase()
{

}

void RendererBase::Initialize()
{

}

void RendererBase::Shutdown()
{

}

void RendererBase::ClearScreen() const
{

}

void RendererBase::DrawColoredLine(const Point2D& point1, const Point2D& point2, const Color& color) const
{

}

void RendererBase::DrawColoredLine(const Point2D& point, const Vector2D& vector, const Color& color) const
{

}

unsigned int RendererBase::GetScreenWidth() const
{
	return m_ScreenWidth;
}

unsigned int RendererBase::GetScreenHeight() const
{
	return m_ScreenHeight;
}
