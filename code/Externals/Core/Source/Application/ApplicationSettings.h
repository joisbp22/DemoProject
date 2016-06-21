#ifndef CORE_APPLICATION_SETTINGS_H
#define CORE_APPLICATION_SETTINGS_H

#include <string>

class ApplicationSettings
{
public:
	
						ApplicationSettings();
						~ApplicationSettings();

	bool				Initialize();

	const std::string&	GetName() const;

	unsigned int		GetScreenWidth() const;
	unsigned int		GetScreenHeight() const;

	unsigned int		GetFramerateMax() const;

private:
	static const std::string	k_SettingsFilePath;

	// default settings
	static const std::string	k_DefaultName;
	static const unsigned int	k_DefaultScreenWidth;
	static const unsigned int	k_DefaultScreenHeight;
	static const unsigned int	k_DefaultFramerateMax;


	std::string		m_Name;
	unsigned int	m_ScreenWidth;
	unsigned int	m_ScreenHeight;
	unsigned int	m_FramerateMax;
};

#endif // CORE_APPLICATION_SETTINGS_H
