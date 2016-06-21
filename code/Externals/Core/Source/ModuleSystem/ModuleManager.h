#ifndef CORE_MODULE_MANAGER_H
#define CORE_MODULE_MANAGER_H

#include <vector>
#include <memory>

#include "Utils/Singleton.h"

#include "Utils/Factory.h"

class ModuleBase;

class ModuleManager : public Singleton<ModuleManager>
{
public:
						ModuleManager();
	virtual				~ModuleManager() override;

	virtual bool		Initialize();
	void				Shutdown();

	void				Update(unsigned long deltaTime);

	template<class T>
	std::weak_ptr<T>	GetModule(unsigned int moduleType) const;

protected:

	Factory<ModuleBase> m_ModuleFactory;

private:

	bool LoadModulesFromXML();

	static const std::string k_SettingsFilePath;

	typedef std::unordered_map<unsigned int, std::shared_ptr<ModuleBase> > ModuleMap;
	ModuleMap m_AllModules;

	std::vector<std::weak_ptr<ModuleBase> > m_UpdateableModules;
};

#define GET_MODULE(ModuleClass) ((ModuleManager::GetInstance<ModuleManager>().expired() == false) ? ModuleManager::GetInstance<ModuleManager>().lock()->GetModule<ModuleClass>(ModuleClass::GetType()) : std::weak_ptr<ModuleClass>())

#include "ModuleManager.hpp"

#endif // CORE_MODULE_MANAGER_H
