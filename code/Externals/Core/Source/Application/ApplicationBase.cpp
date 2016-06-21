#include "ApplicationBase.h"

#include "ModuleSystem/ModuleManager.h"

#include "Utils/time.h"

#include <assert.h>


ApplicationBase::ApplicationBase()
	: Singleton<ApplicationBase>()
{
}

ApplicationBase::~ApplicationBase()
{
}

bool ApplicationBase::Initialize()
{
	bool success = true;

	success &= m_Settings.Initialize();

	std::shared_ptr<ModuleManager> moduleManager = ModuleManager::GetInstance<ModuleManager>().lock();

	assert(moduleManager != nullptr);
	if (moduleManager != nullptr)
	{
		success &= moduleManager->Initialize();
	}
	else
	{
		// no module manager, no game
		success = false;
	}

	return success;
}

void ApplicationBase::Shutdown()
{
	ModuleManager::DestroySingleton();
}

void ApplicationBase::Update(unsigned long deltaTime)
{
	std::shared_ptr<ModuleManager> moduleManager = ModuleManager::GetInstance<ModuleManager>().lock();
	if (moduleManager != nullptr)
	{
		moduleManager->Update(deltaTime);
	}
}

const ApplicationSettings& ApplicationBase::GetSettings() const
{
	return m_Settings;
}
