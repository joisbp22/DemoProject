#include "ModuleBase.h"

IMPLEMENT_FACTORY_INTERFACE(ModuleBase)

ModuleBase::~ModuleBase()
{

}

void ModuleBase::RegisterSerializables(Factory<Serializable>& factory)
{

}

bool ModuleBase::Initialize()
{
	bool success = true;

	return success;
}

void ModuleBase::Shutdown()
{
}

void ModuleBase::Update(unsigned long deltaTime)
{

}

ModuleBase::ModuleBase()
{

}
