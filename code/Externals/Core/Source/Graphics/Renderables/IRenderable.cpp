#include "IRenderable.h"

#include "Graphics/GraphicsModule.h"

IRenderable::IRenderable()
	: Serializable()
{
}

IRenderable::~IRenderable()
{
}

void IRenderable::RegisterRenderable()
{
	std::shared_ptr<GraphicsModule> graphicsModule = GET_MODULE(GraphicsModule).lock();
	if (graphicsModule != nullptr)
	{
		graphicsModule->RegisterRenderable(std::static_pointer_cast<const IRenderable>(m_This.lock()));

		m_GraphicsModule = graphicsModule;
	}
}

void IRenderable::UnregisterRenderable()
{
	std::shared_ptr<GraphicsModule> graphicsModule = m_GraphicsModule.lock();
	if (graphicsModule != nullptr)
	{
		graphicsModule->UnregisterRenderable(std::static_pointer_cast<const IRenderable>(m_This.lock()));
	}
}
