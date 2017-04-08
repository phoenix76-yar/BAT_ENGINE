#include "EngineConfig.h"

#include "../PugiXML/pugixml.hpp"


namespace BEngine
{
	CEngineConfig::CEngineConfig()
	{
		this->loadConfigXMLFile();
	}

	bool CEngineConfig::saveConfig()
	{
		CSpinLockGuard speenlock_guard(m_spinlock);
		if (m_configurationFile != nullptr) 
			return m_configurationFile->save_file(CONFIG_FILE_NAME);
		else 
			return false;	
	}

	bool CEngineConfig::saveConfig(const char* fileName)
	{
		CSpinLockGuard speenlock_guard(m_spinlock);
		if (m_configurationFile != nullptr) 
			return m_configurationFile->save_file(fileName);
		else 
			return false;
	}

	void CEngineConfig::loadConfigXMLFile()
	{
		CSpinLockGuard speenlock_guard(m_spinlock);
		m_configurationFile = new pugi::xml_document();
		auto result = m_configurationFile->load_file(CONFIG_FILE_NAME);
		if (result.status == pugi::xml_parse_status::status_ok)
		{
 			//Get "engine" node from the xml file.
 			auto engineNode = m_configurationFile->child("Engine");
 
 			//Get child node WindowParam
 			auto windowParamNode = engineNode.child("WindowParam");
// 			m_windowParameters.fullScreen = windowParamNode.attribute("fullScreen").as_bool();
// 			m_windowParameters.width = windowParamNode.child("Size").attribute("width").as_int();
// 			m_windowParameters.height = windowParamNode.child("Size").attribute("height").as_int();
// 
 			//Get child node Paths
			auto pathsNode = engineNode.child("Paths");
// 			m_enginePaths.modelsFolderPath = pathsNode.child("Models").attribute("path").as_string();
// 			m_enginePaths.texturesFolderPath = pathsNode.child("Textures").attribute("path").as_string();
		}
		else
		{
			// ****** TODO: Add log here that the configuration file couldn't be read.
			//exit(0);
		}
	}

}