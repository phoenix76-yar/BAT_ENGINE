/***********************************************************************
MASTER SYSTEM

This file holds an implementation of the main system of the engine.
System which is responsible for initializing engine components in a correct,
order, initializing event system and following their life cycle.
At the end shuts down everything, runs on a main thread.


http://gameprogrammingpatterns.com/game-loop.html
***********************************************************************/

#ifndef CMASTERSYSTEM_H
#define CMASTERSYSTEM_H

#include "EngineDefinitions.h"
#include "Core/Event System/IEventListener.h"

namespace BEngine 
{
	class CMSGChannel;
	class IEventListener;
	class IWindow;
	class ILevel;
	
	enum class SystemState
	{
		RUNNING,
		PAUSED
	};


	//In MasterSystemConstrInfo could add also IGame, and other parameters
	//that would configuration the engine.
	struct MasterSystemConstrInfo
	{
		char* configFilePath;

	};

	//-----------------------------------------------------------------------
	//Master System
	//
	//The main component of the whole framework.
	//Responsible for launching engine dependent systems, managing them,
	//updating event system, registering event channels, shutting down
	//and more.
	//-----------------------------------------------------------------------

	class CMasterSystem : public IEventListener
	{
		public:
			CMasterSystem(const MasterSystemConstrInfo&);
			~CMasterSystem();
			
			void initialize() override;
			void shutDown() override;
			bool isComplete() const { return m_isComplete; }

		private:
			void startEngineCycle();
			void processInputMSGs(const bvector<CEventMSG>&) override;

		private:
			SystemState m_sysState;
			bool m_isRunning;
			bool m_isComplete;

			//Engine systems and subsystems

			IWindow* m_engineWindow;
			ILevel*	m_level;

			//Engine core systems
			IEventListener* m_rscManager;
			IEventListener* m_levelManager;

			//Engine systems
			IEventListener* m_renderManager;
			IEventListener* m_physicsManager;
			IEventListener* m_audioManager;
			IEventListener* m_scriptsManager;

			//MSG Channels
			CMSGChannel* m_coreSystemCh;
			CMSGChannel* m_userInputCh;
			CMSGChannel* m_rscChannel;
			CMSGChannel* m_graphicsCh;
			CMSGChannel* m_audioCh;
			CMSGChannel* m_scriptsCh;
			CMSGChannel* m_levelManagerCh;
	};

} //namespace BEngine
#endif //CMASTERSYSTEM_H
