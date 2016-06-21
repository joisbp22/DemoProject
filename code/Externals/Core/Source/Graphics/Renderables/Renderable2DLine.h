#ifndef CORE_RENDERABLE_2D_LINE_H
#define CORE_RENDERABLE_2D_LINE_H

#include "IRenderable.h"

#include "Utils/Factory.h"

#include "Math/Point2D.h"
#include "Utils/Color.h"

class Renderable2DLine : public IRenderable
{
	DECLARE_SERIALIZABLE(Renderable2DLine, Serializable)

public:
	Renderable2DLine();
	virtual ~Renderable2DLine() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void ApplyTransform(const Transform2D& newTransform) override;

	virtual void Render(std::weak_ptr<const RendererBase> renderer) const override;

private:

	float m_Size;

	Color m_Color;

	Point2D m_Points[2];
};

#endif // CORE_RENDERABLE_2D_LINE_H
