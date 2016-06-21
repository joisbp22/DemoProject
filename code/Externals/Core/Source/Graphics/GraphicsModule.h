#ifndef CORE_GRAPHICS_MODULE_H
#define CORE_GRAPHICS_MODULE_H

#include "ModuleSystem/ModuleBase.h"

class RendererBase;
class IRenderable;

class GraphicsModule : public ModuleBase
{
	DECLARE_MODULE(GraphicsModule, ModuleBase)

public:
					GraphicsModule();
	virtual			~GraphicsModule() override;

	virtual void	RegisterSerializables(Factory<Serializable>& factory) override;

	virtual bool	Initialize() override;
	virtual void	Shutdown() override;

	virtual void	Update(unsigned long deltaTime);

	void			RegisterRenderable(std::weak_ptr<const IRenderable> renderable);
	void			UnregisterRenderable(std::weak_ptr<const IRenderable> renderable);

private:

	std::shared_ptr<RendererBase> m_RendererBase;

	std::vector<std::weak_ptr<const IRenderable> > m_Renderables;
};

#endif // CORE_GRAPHICS_MODULE_H