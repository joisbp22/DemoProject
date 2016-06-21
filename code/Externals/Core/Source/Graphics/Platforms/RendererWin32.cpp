#include "RendererWin32.h"

#ifdef WIN32

#undef UNICODE
#include <windows.h>

#include "graphics.h"

#include "Application/Platforms/ApplicationWin32.h"

#include "Math/Point2D.h"
#include "Math/Vector2D.h"

#include "Utils/Color.h"


RendererWin32::RendererWin32()
	: RendererBase()
{

}

RendererWin32::~RendererWin32()
{

}

void RendererWin32::Initialize()
{
	std::shared_ptr<ApplicationWin32> application = ApplicationBase::GetInstance<ApplicationWin32>().lock();
	const ApplicationSettings& settings = application->GetSettings();

	// Initialize graphics library
	// *** do not remove ***
	m_ScreenWidth = settings.GetScreenWidth();
	m_ScreenHeight = settings.GetScreenHeight();

	s_init	init;
	memset(&init, 0, sizeof(init));

	init.captionText = settings.GetName().c_str();
	init.instance = application->GetHInstance();
	init.wndProc = ApplicationWin32::WindowsEventCallback;
	init.screenWidth = m_ScreenWidth;
	init.screenHeight = m_ScreenHeight;

	HWND hwndMain = InitializeGraphics(init);

	ClearScreen();
}

void RendererWin32::Shutdown()
{
	// Uninitialize graphics
	// *** do not remove ***
	UninitializeGraphics();
}

void RendererWin32::ClearScreen() const
{
	// Clear screen
	FillRect(0, 0, m_ScreenWidth, m_ScreenHeight, 0);
}


void RendererWin32::DrawColoredLine(const Point2D& point1, const Point2D& point2, const Color& color) const
{
	int point1X = (int)point1.m_X;
	int point1Y = (int)point1.m_Y;
	int point2X = (int)point2.m_X;
	int point2Y = (int)point2.m_Y;
	DrawLine(point1X, point1Y, point2X, point2Y, color.GetPackedValue());
}

void RendererWin32::DrawColoredLine(const Point2D& point, const Vector2D& vector, const Color& color) const
{
	int pointX = (int)point.m_X;
	int pointY = (int)point.m_Y;
	int vectorX = (int)vector.m_X;
	int vectorY = (int)vector.m_Y;
	DrawLine(pointX, pointY, pointX + vectorX, pointY + vectorY, color.GetPackedValue());
}

#endif // WIN32
