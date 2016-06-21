#ifndef CORE_COMPONENT_VISUAL_H
#define CORE_COMPONENT_VISUAL_H

#include "GameObjectSystem/ComponentBase.h"

class IRenderable;
class ComponentTransform2D;

class ComponentVisual : public ComponentBase
{
	DECLARE_COMPONENT(ComponentVisual, ComponentBase)

public:
	ComponentVisual();
	virtual ~ComponentVisual() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	virtual void Initialize() override;
	virtual void Shutdown() override;

	virtual void Update(unsigned long deltaTime) override;

	virtual void OnSetActive(bool isActive) override;

private:
	std::shared_ptr<IRenderable> m_Renderable;

	std::weak_ptr<ComponentTransform2D> m_TransformComponent;

};

#endif // CORE_COMPONENT_VISUAL_H
