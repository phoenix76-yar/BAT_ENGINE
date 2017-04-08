#include "GLXRender.h"

#include "Buffer Objects/GLXBufferObjects.h"
#include "Camera/Camera.h"
#include "Shader Factory/ShaderManager.h"

#include "glm.hpp"
#include "vec3.hpp"

#include "glew.h"
#include <windows.h>
#include <WinUser.h>
#include <objidl.h>
#include <gdiplus.h>
#include <iostream>

//Temp stuff, just for tests
#include "Shader Factory/GLXShader.h"
#include "Material Factory/Material.h"
#include "Shape Factory/IShape.h"
#include "Shape Factory/Shape Primitives/BoxShape.h"

namespace BEngine
{
	namespace BGraphics
	{
		CGLXRender::CGLXRender() : IRender(BGRenderAPIType::GLX_RENDER)
		{
			m_isInitialized = false;
		}

		CGLXRender::~CGLXRender()
		{
			//this->shutDown();
		}

		bool CGLXRender::initialize(const BGRenderConstrInfo& cInfo)
		{
			if (!m_isInitialized)
			{
				m_renderAreaX = cInfo.width;
				m_renderAreaY = cInfo.height;

				m_windowHandle = cInfo.windowHandle;
				m_hangleDeviceContext = GetDC((HWND)m_windowHandle);

				PIXELFORMATDESCRIPTOR pfd;
				pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
				pfd.nVersion = 1;
				pfd.iLayerType = PFD_MAIN_PLANE;
				pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
				pfd.cColorBits = 32;
				pfd.cDepthBits = 24;
				pfd.cStencilBits = 8;
				pfd.iPixelType = PFD_TYPE_RGBA;

				int pixelFormat = ChoosePixelFormat((HDC)m_hangleDeviceContext, &pfd);

				if (!SetPixelFormat((HDC)m_hangleDeviceContext, pixelFormat, &pfd))
				{
					MessageBoxA(NULL, "Could not set pixel format!", "Error!", MB_ICONEXCLAMATION | MB_OK);
					//return EResult::E_ERROR;
					return false;
				}

				m_handleRenderingContext = wglCreateContext((HDC)m_hangleDeviceContext);
				if (!wglMakeCurrent((HDC)m_hangleDeviceContext, (HGLRC)m_handleRenderingContext))
				{
					wglDeleteContext((HGLRC)m_handleRenderingContext);
					return false;
				}

				GLenum err = glewInit();
				if (GLEW_OK != err)
				{
					MessageBoxA(NULL, "OpenGL Error!", "Error!", MB_ICONEXCLAMATION | MB_OK);
					//return EResult::E_ERROR;
					return false;
				}

				//checkSpecs();

				float aspectRatio = (float)m_renderAreaX / (float)m_renderAreaY;
				// ****** NOTE: When use inversed depth buffer swap near plane with far plane - swap 0.1f with 10000.0f !
				const CameraConstrInfo cameraCInfo
				{
					0.7854f, //In Radians (45 degrees is around 0.7854 radians, degrees * PI/180)
					aspectRatio,
					0.1f,
					10000.0f
				};

				m_camera = new OrbitCamera(cameraCInfo);
				m_camera->initialize();

				//Set-up render info struct
				m_renderInfo.renderPtr = this;
				m_renderInfo.cameraPtr = m_camera;

				//Create an empty camera buffer
				BG1BufferConstrInfo camBuffConstrInfo
				{
					nullptr,
					CAMERA_BUFFER_SIZE, 
					nullptr,
					BG1BufferType::CONSTANT_UNIFORM_BUFFER,
					0
				};
				m_camUniformBuffer = new BG1GLXBuffer(camBuffConstrInfo);

				glViewport(0, 0, m_renderAreaX, m_renderAreaY);
				glEnable(GL_DEPTH_TEST);

				glEnable(GL_BLEND);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);

				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_MULTISAMPLE);
				glEnable(GL_TEXTURE_2D);

				m_shaderManager = new CShaderManager();
				m_shaderManager->initialize();

				// ****** NOTE: Just for debugging purposes.
				PritimiveShapeConstrInfo sCInfo;
				sCInfo.shaderType = SHADER_PRIMITIVE;
				sCInfo.shapeType = PrimitiveShapeTypes::S_BOX;			
				sCInfo.info.BoxShapeInfo.height = 2.0f;
				sCInfo.info.BoxShapeInfo.length = 2.0f;
				sCInfo.info.BoxShapeInfo.width = 2.0f;
				
#ifdef _DEBUG
				uint32 count = 20;
#else
				uint32 count = 40;
#endif
				float distAppart = 6.0f;
				for (auto i = 0; i < count; i++)
				{
					for (auto j = 0; j < count; j++)
					{
						IShape* testShape;
						this->createShape(&testShape, sCInfo);
						testShape->setPosition(glm::vec3(-((distAppart * count) / 2) + distAppart * i, 
							-((distAppart * count) / 2) + distAppart * j, 0.0f));
						testShape->initialize(this->m_renderInfo);
						this->addStaticShape(testShape);
					}	
				}

				return true;
			}
			return true;
		}

		void CGLXRender::shutDown()
		{

			//Clear shapes if you have forgotten to remove them by yourself
			while (!m_staticShapeCont.empty())
			{
				if (m_staticShapeCont.back())
				{
					delete m_staticShapeCont.back();
				}
				m_staticShapeCont.pop_back();
			}

			while (!m_dynamicShapeCont.empty())
			{
				if (m_dynamicShapeCont.back())
				{
					delete m_dynamicShapeCont.back();
				}
				m_dynamicShapeCont.pop_back();
			}

			if (m_camUniformBuffer) delete m_camUniformBuffer;
			if (m_camera) delete m_camera;

			if (m_shaderManager)
			{
				m_shaderManager->shutDown();
				delete m_shaderManager;
			}

			wglMakeCurrent(NULL, NULL);
			if (!wglDeleteContext((HGLRC)m_handleRenderingContext))
			{
				MessageBoxA(NULL, "Could not delete draw context!", "Error!", MB_ICONEXCLAMATION | MB_OK);
			}
			if (!ReleaseDC((HWND)m_windowHandle, (HDC)m_hangleDeviceContext))
			{
				MessageBoxA(NULL, "Could not release handle device context!", "Error!", MB_ICONEXCLAMATION | MB_OK);
			}
		}

		void CGLXRender::update()
		{
			//Before rendering anything update constant buffers for the next frame, like camera buffer, etc.
			if (m_camera != nullptr && m_camUniformBuffer != nullptr)
			{
				memcpy(&m_camRamBuffer[0], &m_camera->getViewMatrix()[0][0], 64);
				memcpy(&m_camRamBuffer[64], &m_camera->getProjectionMatrix()[0][0], 64);

				m_camUniformBuffer->mapBuffer(m_camUniformBuffer, CAMERA_BUFFER_SIZE);
				m_camUniformBuffer->unmapBuffer();

				//Update static shapes
				for (auto shContIt = m_staticShapeCont.begin(); 
				shContIt != m_staticShapeCont.end(); ++shContIt)
				{
					//(*shContIt)->setRotationByAngle(glm::vec3(0, 0, 1), 0.5);
					if ((*shContIt)->getDrawShapeFlag())
					{
						(*shContIt)->update(m_renderInfo);
					}	
				}
			}
		}

		inline float getDistSq(const glm::vec3& pos1, const glm::vec3& pos2)
		{
			return ((pos2.x - pos1.x) * (pos2.x - pos1.x) + 
				(pos2.y - pos1.y) * (pos2.y - pos1.y) + 
				(pos2.z - pos1.z) * (pos2.z - pos1.z));
		}

		void CGLXRender::render()
		{
			//m_hangleDeviceContext = GetDC((HWND)m_windowHandle);
 			//wglMakeCurrent((HDC)m_hangleDeviceContext, (HGLRC)m_handleRenderingContext);

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
			glClearColor(0.24f, 0.24f, 0.24f, 1.0f);
			//glClearColor(0.5f, 0.15f, 0.7f, 1.0f); //0.2 instead of 0.5

			//If we don't have camera or camera uniform buffer is not created we can't draw!
			if (m_camera != nullptr && m_camUniformBuffer != nullptr)
			{
				m_camUniformBuffer->bindForDraw(CAMERA_BUFFER_SIZE, 0);
				for (auto shContIt = m_staticShapeCont.begin(); 
					shContIt != m_staticShapeCont.end(); ++shContIt)
				{
					if ((*shContIt)->getDrawShapeFlag())
					{
						float dist = getDistSq((*shContIt)->getPosition(), m_camera->getCameraPos());
						float drawDist = (*shContIt)->getDrawDist();
						if ((drawDist * drawDist) >= dist)
						{
							(*shContIt)->draw(m_renderInfo);
						}
					}				
				}
				m_camUniformBuffer->unbind();
			}
			SwapBuffers((HDC)m_hangleDeviceContext);
		}

		void CGLXRender::resize(uint32 width, uint32 height)
		{
			m_renderAreaX = width;
			m_renderAreaY = height;
			glViewport(0, 0, m_renderAreaX, m_renderAreaY);

 			if (m_camera != nullptr)
 			{
 				float aspectRatio = 1.0f * (float)width / (float)height;
 				m_camera->setAspectRatio(aspectRatio);
 			}
			
		}

		void CGLXRender::drawElements(BG1PrimitiveTopology primTopology, uint32 indexCount, const void* indices)
		{
			switch (primTopology)
			{
			case BG1PrimitiveTopology::BG_LINES:
			{
				glDrawElements(GL_LINES, indexCount, GL_UNSIGNED_INT, indices);
				break;
			}
			case BG1PrimitiveTopology::BG_TRIANGLES:
			{
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
				break;
			}
			case BG1PrimitiveTopology::BG_TRIANGLE_STRIP:
			{
				glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, indices);
				break;
			}
			default:
				break;
			}
		}

		void CGLXRender::drawInstanced(BG1PrimitiveTopology primTopology, uint32 indexCount, uint32 instanceCount, const void* indices)
		{
			switch (primTopology)
			{
			case BG1PrimitiveTopology::BG_LINES:
			{
				glDrawElementsInstanced(GL_LINES, indexCount, GL_UNSIGNED_INT, indices, instanceCount);
				break;
			}
			case BG1PrimitiveTopology::BG_TRIANGLES:
			{
				glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices, instanceCount);
				break;
			}
			case BG1PrimitiveTopology::BG_TRIANGLE_STRIP:
			{
				glDrawElementsInstanced(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, indices, instanceCount);
				break;
			}
			default:
				break;
			}
		}
	}
}
