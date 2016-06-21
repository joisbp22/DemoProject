#ifndef CORE_MODULE_BASE_H
#define CORE_MODULE_BASE_H

#include "ModuleManager.h"

#include "Utils/Factory.h"
#include "Serialization/Serializable.h"

class ModuleBase
{
	DECLARE_FACTORY_INTERFACE(ModuleBase)

public:
	virtual ~ModuleBase();

	virtual void RegisterSerializables(Factory<Serializable>& factory);

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Update(unsigned long deltaTime);

protected:
	ModuleBase();
};

#define DECLARE_MODULE(ModuleName, BaseClass) \
	DECLARE_FACTORY_CONSTRUCTABLE(ModuleName, BaseClass, ModuleBase) \
	private:

#define IMPLEMENT_MODULE(ModuleName) \
	IMPLEMENT_FACTORY_CONSTRUCTABLE(ModuleName, ModuleBase)

#endif // CORE_MODULE_BASE_H