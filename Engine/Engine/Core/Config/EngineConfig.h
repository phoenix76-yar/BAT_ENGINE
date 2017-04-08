#ifndef ENGINE_CONFIG_H
#define ENGINE_CONFIG_H

#include "../Templates/Noncopyable.h"
#include "../Multi Threading/Spinlock.h"

//Pugi XML document class forward declaration
namespace pugi { class xml_document; }

namespace BEngine
{
	static constexpr char* CONFIG_FILE_NAME = "..\\..\\config.xml";

	//-----------------------------------------------------------------------
	//CEngineConfig Class
	//
	//IMPORTANT THIS CLASS MUST BE INITIALISED BEFORE OTHER ENGINE COMPONENTS
	//ARE INITIALISED !!! THE ONLY EXCEPTION IS THE LOGGER CLASS!
	//TO INITIALISE BCONFIG JUST CALL: CBConfig::getInstnace();
	//For more detailed description see file header.
	//-----------------------------------------------------------------------

	class CEngineConfig : public CNoncopyable
	{
		public:
			static CEngineConfig& getInstance() 
			{
				static CEngineConfig cInstance;
				return cInstance;
			}
				
			bool saveConfig();
			bool saveConfig(const char* fileName);

		private:
			void loadConfigXMLFile();
			CEngineConfig();
			~CEngineConfig() = default;

			pugi::xml_document* m_configurationFile;
			CSpinLock m_spinlock;		
	};
}

#endif //ENGINE_CONFIG_H