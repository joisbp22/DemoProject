#ifndef CORE_IRENDERABLE_H
#define CORE_IRENDERABLE_H

#include "Serialization/Serializable.h"

class Transform2D;
class RendererBase;
class GraphicsModule;

class IRenderable : public Serializable
{
public:
	virtual ~IRenderable() override;

	void RegisterRenderable();
	void UnregisterRenderable();

	virtual void ApplyTransform(const Transform2D& newTransform) = 0;

	virtual void Render(std::weak_ptr<const RendererBase> renderer) const = 0;

protected:
	IRenderable();

private:
	std::weak_ptr<GraphicsModule> m_GraphicsModule;
};

#endif // CORE_IRENDERABLE_H
