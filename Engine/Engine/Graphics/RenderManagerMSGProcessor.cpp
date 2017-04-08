#include "RenderManager.h"
#include "IRender.h"
#include "GLXRender.h"
#include "Shader Factory/ShaderManager.h"

#include "Camera/Camera.h"


#include "GraphicsMSGs.h"

#include "../Window/WindowMSG.h"


#include "vec2.hpp"

namespace BEngine
{
	namespace BGraphics
	{
		void printDebugInfo(const char* info)
		{
#ifdef _DEBUG
			//std::cout << info << endl;
#endif
		}

		// ****** TODO: Put those processInputMSGs into separate .cpp files!
		void CRenderManager::processInputMSGs(const bvector<CEventMSG>& msgCont)
		{
			while (!m_msgCont.empty())
			{
				switch (m_msgCont.back().m_msgType)
				{
				case EventMSGType::UserInput:
				{
					auto inMsg = m_msgCont.back();
					this->processUserInputMSG(inMsg);
					break;
				}
				case EventMSGType::GraphicsData:
				{
					auto inMsg = m_msgCont.back();
					this->processGraphicsMSG(inMsg);
					break;
				}
				default:
					break;
				}
				m_msgCont.pop_back();
			}

		}


		void CRenderManager::processUserInputMSG(const CEventMSG& inMsg)
		{
			auto userData = inMsg.userDataBuffer;
			auto inputData = *(CWindowMSG*)userData;
			if (inputData.msgType == WindowMSGTypes::USER_INPUT)
			{
				printDebugInfo("WindowMSGTypes::USER_INPUT\n");
				switch (inputData.winMsg.userInput.inAction)
				{
				case InputAction::IA_MOUSE_WHEEL_SCROLL_UP:
				{
					auto camera = m_render->getActiveCamera();
					if (camera != nullptr && camera->getCameraType() == CameraType::ORBIT)
					{
						float zoomDist = 5.15f;
						if (m_render->getActiveCamera()->getDistBetweenCameraAndTarget() < 25.0f)
						{
							zoomDist = 1.15f;
						}
						m_render->getActiveCamera()->setDistanceBetweenCamAndTarget(m_render->getActiveCamera()->getDistBetweenCameraAndTarget() - zoomDist);
					}

					printDebugInfo("InputAction::IA_MOUSE_WHEEL_SCROLL_UP\n");
					break;
				}
				case InputAction::IA_MOUSE_WHEEL_SCROLL_DOWN:
				{
					auto camera = m_render->getActiveCamera();
					if (camera != nullptr && camera->getCameraType() == CameraType::ORBIT)
					{
						float zoomDist = 5.15f;
						if (m_render->getActiveCamera()->getDistBetweenCameraAndTarget() < 25.0f)
						{
							zoomDist = 1.15f;
						}
						m_render->getActiveCamera()->setDistanceBetweenCamAndTarget(m_render->getActiveCamera()->getDistBetweenCameraAndTarget() + zoomDist);
					}
					printDebugInfo("InputAction::IA_MOUSE_WHEEL_SCROLL_DOWN\n");
					break;
				}
				case InputAction::IA_MOUSE_MOVE:
				{
					printDebugInfo("InputAction::IA_MOUSE_MOVE\n");
					// 							std::cout << "MOUSE_POS X: " << inputData.winMsg.userInput.mousePosX <<
					// 								" Y: " << inputData.winMsg.userInput.mousePosY << std::endl;

					auto camera = m_render->getActiveCamera();
					if (camera != nullptr && camera->getCameraType() == CameraType::ORBIT && m_xKeyDown == true)
					{
						float zoomDist = 5.15f;
						((OrbitCamera*)camera)->updateMousePos(glm::vec2(inputData.winMsg.userInput.mousePosX, inputData.winMsg.userInput.mousePosY));
					}
					break;
				}
				case InputAction::IA_WINDOW_MINMIZED:
				{
					m_pauseRendering = true;
					printDebugInfo("InputAction::IA_WINDOW_MINMIZED -------- RENDERING STOPPED\n");
					break;
				}
				case InputAction::IA_WINDOW_RESTORED:
				{
					m_pauseRendering = false;
					printDebugInfo("InputAction::IA_WINDOW_RESTORED -------- RENDERING RESTORED\n");
					break;
				}
				case InputAction::IA_KEY_DOWN:
				{
					switch (inputData.winMsg.userInput.inKeyID)
					{
					case KeyID::KEY_X:
					{
						m_xKeyDown = true;
						break;
					}
					default:
						break;
					}
					break;
				}
				case InputAction::IA_KEY_UP:
				{

					switch (inputData.winMsg.userInput.inKeyID)
					{
					case KeyID::KEY_X:
					{
						m_xKeyDown = false;
						break;
					}
					default:
						break;
					}
					break;
				}
				default:
					break;
				}
			}
			else if (inputData.msgType == WindowMSGTypes::WINDOW_RESIZE)
			{
				printDebugInfo("WindowMSGTypes::WINDOW_RESIZE\n");
				if (m_render != nullptr)
				{
					m_render->resize(inputData.winMsg.winResize.clientWidth,
						inputData.winMsg.winResize.cleintHeight);
				}
			}
		}

		void CRenderManager::processGraphicsMSG(const CEventMSG& inMsg)
		{
			auto userData = inMsg.userDataBuffer;
			auto inputData = *(CGraphicsMSG*)userData;

			switch (inputData.msgType)
			{
			case GraphicsMSGType::GR_ADD_SHAPE:
			{

				break;
			}
			case GraphicsMSGType::GR_ADD_LIGHT:
			{

				break;
			}
			default:
				break;
			}

		}
	} //namespace BGraphics
} //BEngine