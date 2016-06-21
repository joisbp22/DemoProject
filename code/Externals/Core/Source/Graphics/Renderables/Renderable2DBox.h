#ifndef CORE_RENDERABLE_2D_BOX_H
#define CORE_RENDERABLE_2D_BOX_H

#include "IRenderable.h"

#include "Utils/Factory.h"

#include "Math/Point2D.h"
#include "Utils/Color.h"

class Renderable2DBox : public IRenderable
{
	DECLARE_SERIALIZABLE(Renderable2DBox, Serializable)

public:
	Renderable2DBox();
	virtual ~Renderable2DBox() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void ApplyTransform(const Transform2D& newTransform) override;

	virtual void Render(std::weak_ptr<const RendererBase> renderer) const override;

private:
	float m_HalfSize;
	Color m_Color;

	Point2D m_Points[4];
};

#endif // CORE_RENDERABLE_2D_BOX_H
