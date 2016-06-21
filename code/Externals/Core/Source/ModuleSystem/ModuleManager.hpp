#ifndef CORE_MODULE_MANAGER_HPP
#define CORE_MODULE_MANAGER_HPP

template <class T>
std::weak_ptr<T> ModuleManager::GetModule(unsigned int moduleType) const
{
	ModuleMap::const_iterator it = m_AllModules.find(moduleType);
	if (it != m_AllModules.end())
		return std::static_pointer_cast<T>(it->second);

	return std::weak_ptr<T>();
}

#endif // CORE_MODULE_MANAGER_HPP
