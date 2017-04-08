#include "RenderManager.h"
#include "IRender.h"
#include "GLXRender.h"
#include "Shader Factory/ShaderManager.h"
//#include "Shader Factory/GLXShader.h"

#include "Camera/Camera.h"
#include "vec2.hpp"

namespace BEngine
{
	namespace BGraphics
	{
		CRenderManager::CRenderManager(const BGRenderConstrInfo & cInfo)
		{
			m_renderCInfo = cInfo;
			m_pauseRendering = false;
		}

		CRenderManager::~CRenderManager()
		{
			if (m_threadState.load())
			{
				shutDown();
			}
		}

		void CRenderManager::update()
		{
			m_render = new CGLXRender();
			auto result = m_render->initialize(m_renderCInfo);

			if (result) //== RESSULT::OK
			{
				//Initialize all render subsystems (MRT - deffered renderer, time cycle, etc.)

			}

			while (m_threadState.load())
			{
				copyOverMSGs(m_msgCont);
				processInputMSGs(m_msgCont);
				
				//Process MSG messages, look for resize window messages, etc.
				//Shape, light and other messages
				m_render->processUnusedShapes();
				m_render->processLODGroups();
				
				//Sort shapes by LOD
				m_render->swapLODS();

				//Sort solid shapes front to back (for better rendering performance)
				m_render->sortSolidFrontToBack();

				//Sort transparent shapes back to front
				m_render->sortTransparentBackToFront();

				//Occlusion (shapes being occluded by other shapes in front)

				m_render->update();
				if (!m_pauseRendering)
				{
					m_render->render();
				}


				//std::this_thread::sleep_for(std::chrono::microseconds(5));
			}

			//Shut down all subsystems in reverse order
			
			m_render->shutDown(); //should be the last one to shut down, at this stage no more update or anything ...

			//delete all subsystems
			delete m_render;
			m_isInitialized = false;
		}
	}
}
