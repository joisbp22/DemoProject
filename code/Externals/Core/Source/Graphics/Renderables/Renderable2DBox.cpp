#include "Renderable2DBox.h"

#include "Serialization/SerializationModule.h"

#include "Graphics/RendererBase.h"
#include "Math/Transform2D.h"


IMPLEMENT_SERIALIZABLE(Renderable2DBox)

Renderable2DBox::Renderable2DBox()
{

}

Renderable2DBox::~Renderable2DBox()
{

}

bool Renderable2DBox::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_HalfSize, Renderable2DBox);
	assert(success);

	success &= DESERIALIZE_VARIABLE(m_Color, Renderable2DBox);
	assert(success);

	return success;
}

void Renderable2DBox::ApplyTransform(const Transform2D& newTransform)
{
	Vector2D upScaled = newTransform.m_Matrix.GetUpVector() * newTransform.m_Scale * m_HalfSize;
	Vector2D rightScaled = newTransform.m_Matrix.GetRightVector() * newTransform.m_Scale * m_HalfSize;

	m_Points[0] = newTransform.m_Position + upScaled + rightScaled;
	m_Points[1] = newTransform.m_Position + upScaled - rightScaled;
	m_Points[2] = newTransform.m_Position - upScaled + rightScaled;
	m_Points[3] = newTransform.m_Position - upScaled - rightScaled;
}

void Renderable2DBox::Render(std::weak_ptr<const RendererBase> renderer) const
{
	std::shared_ptr<const RendererBase> rendererShared = renderer.lock();
	if (rendererShared != nullptr)
	{
		rendererShared->DrawColoredLine(m_Points[0], m_Points[2], m_Color);
		rendererShared->DrawColoredLine(m_Points[2], m_Points[3], m_Color);
		rendererShared->DrawColoredLine(m_Points[3], m_Points[1], m_Color);
		rendererShared->DrawColoredLine(m_Points[1], m_Points[0], m_Color);
	}
}
