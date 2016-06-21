#ifndef CORE_APPLICATION_BASE_H
#define CORE_APPLICATION_BASE_H

#include "Utils/Singleton.h"

#include "ApplicationSettings.h"

class ModuleManager;

class ApplicationBase : public Singleton<ApplicationBase>
{
public:
	
								ApplicationBase();
	virtual						~ApplicationBase();

	const ApplicationSettings&	GetSettings() const;

protected:

	virtual bool				Initialize();
	virtual void				Shutdown();

	virtual void				Update(unsigned long deltaTime);

private:
	ApplicationSettings m_Settings;
};

#endif // CORE_APPLICATION_BASE_H
