#ifndef CORE_RENDERABLE_2D_TRIANGLE_H
#define CORE_RENDERABLE_2D_TRIANGLE_H

#include "IRenderable.h"

#include "Utils/Factory.h"

#include "Math/Point2D.h"
#include "Utils/Color.h"

class Renderable2DTriangle : public IRenderable
{
	DECLARE_SERIALIZABLE(Renderable2DTriangle, Serializable)

public:
	Renderable2DTriangle();
	virtual ~Renderable2DTriangle() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void ApplyTransform(const Transform2D& newTransform) override;

	virtual void Render(std::weak_ptr<const RendererBase> renderer) const override;

private:

	float m_BaseSize;
	float m_Height;

	Color m_Color;

	Point2D m_Points[3];
};

#endif // CORE_RENDERABLE_2D_TRIANGLE_H
