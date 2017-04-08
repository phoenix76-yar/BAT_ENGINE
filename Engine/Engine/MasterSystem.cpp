#include "MasterSystem.h"

//Event system related includes
#include "Core/Event System/EventManager.h"
#include "Core/Event System/IEventListener.h"
#include "Core/Event System/MSGChannel.h"
#include "SystemMSGs.h"

//Resource management systems
#include "Core/Resource Manager/RSCManager.h"

//Window management includes
#include "Window/Win32Window.h"

//Graphics pipeline includes
#include "Graphics/RenderManager.h"

//Physics includes
#include "Physics/PhysicsManager.h"

//Audio includes
#include "Audio/AudioManager.h"

//Level Manager includes
#include "Core/Level Manager/SimpleLevel.h"

#include <iostream>
#include <thread>

namespace BEngine
{
	void initWindow(IWindow* window)
	{
		window->initialize();
		window->update();
		delete window;
	}

	CMasterSystem::CMasterSystem(const MasterSystemConstrInfo& cInfo)
	{
		CEventManager::getInstance().initialize();
	}

	CMasterSystem::~CMasterSystem(){}

	void CMasterSystem::initialize()
	{
		std::cout << "Number of available threads: " << std::thread::hardware_concurrency() << std::endl;

		WindowConstrInfo winCInfo
		{
			1400,
			900
		};

		m_engineWindow = new CWin32Window(winCInfo);
		std::thread windowThread(initWindow, m_engineWindow);

		std::this_thread::sleep_for(std::chrono::microseconds(5));
		while (!m_engineWindow->isInitialized()) { std::this_thread::sleep_for(std::chrono::microseconds(5)); }

		m_rscManager = new CRSCManager;
		m_rscManager->initialize();

		BGraphics::BGRenderConstrInfo renderCInfo
		{
			m_engineWindow->getWindowHandle(),
			1400,
			900,
			false,
			true
		};

		m_renderManager = new BGraphics::CRenderManager(renderCInfo);		
		m_physicsManager = new BPhysics::CPhysicsManager();
		m_audioManager = new BAudio::CAudioManager();

		//Create communication channels between systems
		m_coreSystemCh = new CMSGChannel(EventMSGType::System);
		m_userInputCh = new CMSGChannel(EventMSGType::UserInput);
		m_rscChannel = new CMSGChannel(EventMSGType::ResourceData);
		m_audioCh = new CMSGChannel(EventMSGType::AudioData);
		m_graphicsCh = new CMSGChannel(EventMSGType::GraphicsData);

		m_coreSystemCh->registerSystem(this);
		m_rscChannel->registerSystem(m_rscManager);
		m_userInputCh->registerSystem(m_renderManager);
		m_audioCh->registerSystem(m_audioManager);
		m_graphicsCh->registerSystem(m_renderManager);

		EventManager.registerChannel(m_coreSystemCh);
		EventManager.registerChannel(m_userInputCh);
		EventManager.registerChannel(m_rscChannel);
		EventManager.registerChannel(m_audioCh);
		EventManager.registerChannel(m_graphicsCh);

		m_renderManager->initialize();
		m_physicsManager->initialize();
		m_audioManager->initialize();

		m_isRunning = true;	
		m_sysState = SystemState::RUNNING;

		m_level = new CSimpleLevel();
		LevelUpdateInfo lInfo;
		m_level->loadLevel(lInfo);

		//this_thread::sleep_for(std::chrono::seconds(5));

		this->startEngineCycle();
		windowThread.join();
	}

	void CMasterSystem::shutDown()
	{
		m_isRunning = false;
	}

	static LevelUpdateInfo lInfo2;

	void CMasterSystem::startEngineCycle()
	{
		while (m_isRunning)
		{
			EventManager.updateBeforeFrame();
			copyOverMSGs(m_msgCont);
			processInputMSGs(m_msgCont);

			m_level->update(lInfo2);

			std::this_thread::sleep_for(std::chrono::microseconds(10)); // ****** TODO: to be removed.
			EventManager.updateAfterFrame();
		}

		m_level->shutDown();
		m_renderManager->shutDown();
		m_physicsManager->shutDown();
		m_audioManager->shutDown();
		m_rscManager->shutDown();
		m_engineWindow->shutDown();
	}
} //namespace BEngine