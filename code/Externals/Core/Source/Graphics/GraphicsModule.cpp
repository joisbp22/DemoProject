#include "GraphicsModule.h"

#ifdef WIN32
#include "Platforms/RendererWin32.h"
#endif // WIN32

#include "Renderables/Renderable2DTriangle.h"
#include "Renderables/Renderable2DBox.h"
#include "Renderables/Renderable2DLine.h"


IMPLEMENT_MODULE(GraphicsModule)

GraphicsModule::GraphicsModule()
	: ModuleBase()
	, m_RendererBase(nullptr)
{
#ifdef WIN32
	m_RendererBase = std::make_shared<RendererWin32>();
#endif // WIN32
}

GraphicsModule::~GraphicsModule()
{

}

void GraphicsModule::RegisterSerializables(Factory<Serializable>& factory)
{
	REGISTER_FACTORY_CONSTRUCTABLE(Renderable2DTriangle, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(Renderable2DBox, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(Renderable2DLine, factory);
}

bool GraphicsModule::Initialize()
{
	assert(m_RendererBase != nullptr);
	if (m_RendererBase != nullptr)
	{
		m_RendererBase->Initialize();

		return true;
	}

	return false;
}

void GraphicsModule::Shutdown()
{
	assert(m_RendererBase != nullptr);
	if (m_RendererBase != nullptr)
	{
		m_RendererBase->Shutdown();
	}
}

void GraphicsModule::Update(unsigned long deltaTime)
{
	m_RendererBase->ClearScreen();

	for (const auto it : m_Renderables)
	{
		std::shared_ptr<const IRenderable> renderable = it.lock();
		if (renderable != nullptr)
		{
			renderable->Render(m_RendererBase);
		}
	}
}

void GraphicsModule::RegisterRenderable(std::weak_ptr<const IRenderable> renderable)
{
	UnregisterRenderable(renderable);
	m_Renderables.push_back(renderable);
}

void GraphicsModule::UnregisterRenderable(std::weak_ptr<const IRenderable> renderable)
{
	std::shared_ptr<const IRenderable> renderableShared = renderable.lock();

	std::vector<std::weak_ptr<const IRenderable> >::iterator it = m_Renderables.begin();
	for (; it != m_Renderables.end(); )
	{
		if (it->lock() == renderableShared)
		{
			it = m_Renderables.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}
}
