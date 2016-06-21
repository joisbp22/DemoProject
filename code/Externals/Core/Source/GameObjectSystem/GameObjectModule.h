#ifndef CORE_GAME_OBJECT_MODULE_H
#define CORE_GAME_OBJECT_MODULE_H

#include "ModuleSystem/ModuleBase.h"

#include <memory>

class GameObject;
class ComponentTransform2D;

class GameObjectModule : public ModuleBase
{
	DECLARE_MODULE(GameObjectModule, ModuleBase)

public:
					GameObjectModule();
	virtual			~GameObjectModule() override;

	virtual void	RegisterSerializables(Factory<Serializable>& factory) override;

	virtual bool	Initialize() override;
	virtual void	Shutdown() override;

	virtual void	Update(unsigned long deltaTime) override;

	bool			LoadScene(const std::string& sceneFilePath);
	std::weak_ptr<GameObject>	CloneObject(std::weak_ptr<GameObject> object);

	void			RegisterModifiedTransformComponent(std::weak_ptr<ComponentTransform2D> modifiedTransform);

private:

	void			UpdateTransforms();

	bool			LoadPrefabs(const std::string& prefabsFilePath);

	std::shared_ptr<GameObject> m_Root;
	std::shared_ptr<GameObject> m_Prefabs;

	std::vector<std::weak_ptr<ComponentTransform2D> > m_ModifiedTransforms;
};

#endif // CORE_GAME_OBJECT_MODULE_H
