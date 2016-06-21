#include "Renderable2DLine.h"

#include "Serialization/SerializationModule.h"

#include "Graphics/RendererBase.h"
#include "Math/Transform2D.h"


IMPLEMENT_SERIALIZABLE(Renderable2DLine)

Renderable2DLine::Renderable2DLine()
{

}

Renderable2DLine::~Renderable2DLine()
{

}

bool Renderable2DLine::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_Size, Renderable2DLine);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Color, Renderable2DLine);
	assert(success);

	return success;
}

void Renderable2DLine::ApplyTransform(const Transform2D& newTransform)
{
	Vector2D upScaled = newTransform.m_Matrix.GetUpVector() * newTransform.m_Scale * m_Size;

	m_Points[0] = newTransform.m_Position;
	m_Points[1] = newTransform.m_Position + upScaled;
}

void Renderable2DLine::Render(std::weak_ptr<const RendererBase> renderer) const
{
	std::shared_ptr<const RendererBase> rendererShared = renderer.lock();
	if (rendererShared != nullptr)
	{
		rendererShared->DrawColoredLine(m_Points[0], m_Points[1], m_Color);
	}
}
