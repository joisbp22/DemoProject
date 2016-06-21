#include "Renderable2DTriangle.h"

#include "Serialization/SerializationModule.h"

#include "Graphics/RendererBase.h"
#include "Math/Transform2D.h"


IMPLEMENT_SERIALIZABLE(Renderable2DTriangle)

Renderable2DTriangle::Renderable2DTriangle()
{

}

Renderable2DTriangle::~Renderable2DTriangle()
{

}

bool Renderable2DTriangle::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_BaseSize, Renderable2DTriangle);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Height, Renderable2DTriangle);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_Color, Renderable2DTriangle);
	assert(success);

	return success;
}

void Renderable2DTriangle::ApplyTransform(const Transform2D& newTransform)
{
	Vector2D upScaled = newTransform.m_Matrix.GetUpVector() * newTransform.m_Scale * m_Height;
	Vector2D rightScaled = newTransform.m_Matrix.GetRightVector() * newTransform.m_Scale * (m_BaseSize / 2.0f);

	m_Points[0] = newTransform.m_Position + upScaled;
	m_Points[1] = newTransform.m_Position - rightScaled;
	m_Points[2] = newTransform.m_Position + rightScaled;
}

void Renderable2DTriangle::Render(std::weak_ptr<const RendererBase> renderer) const
{
	std::shared_ptr<const RendererBase> rendererShared = renderer.lock();
	if (rendererShared != nullptr)
	{
		rendererShared->DrawColoredLine(m_Points[0], m_Points[1], m_Color);
		rendererShared->DrawColoredLine(m_Points[1], m_Points[2], m_Color);
		rendererShared->DrawColoredLine(m_Points[2], m_Points[0], m_Color);
	}
}
