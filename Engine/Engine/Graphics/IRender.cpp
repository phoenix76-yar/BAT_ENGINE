#include "IRender.h"
#include "Camera/Camera.h"

//For shape create.
#include "Shader Factory/GLXShader.h"
#include "Material Factory/Material.h"
#include "Shape Factory/IShape.h"
#include "Shader Factory/ShaderManager.h"
#include "Shape Factory/Shape Primitives/BoxShape.h"

namespace BEngine
{
	namespace BGraphics
	{
		void IRender::addStaticShape(IShape* shape)
		{
			if (shape != nullptr)
			{
				m_staticShapeCont.push_back(shape);
			}
		}

		void IRender::removeStaticShpe(IShape* shape)
		{
			if (shape != nullptr)
			{
				for (auto m_staticShapeContIt = m_staticShapeCont.begin(); 
				m_staticShapeContIt != m_staticShapeCont.end(); ++m_staticShapeContIt)
				{
					if (shape == (*m_staticShapeContIt))
					{
						m_staticShapeContIt = m_staticShapeCont.erase(m_staticShapeContIt);
						if (m_staticShapeContIt == m_staticShapeCont.end() && m_staticShapeCont.size() == 0)
						{
							break;
						}
					}
				}
			}
		}

		void IRender::addDynamicShape(IShape* shape)
		{
			if (shape != nullptr)
			{
				m_dynamicShapeCont.push_back(shape);
			}
		}

		void IRender::removeDynamicShape(IShape* shape)
		{
			if (shape != nullptr)
			{
				for (auto m_dynamicShapeContIt = m_dynamicShapeCont.begin(); 
				m_dynamicShapeContIt != m_dynamicShapeCont.end(); ++m_dynamicShapeContIt)
				{
					if (shape == (*m_dynamicShapeContIt))
					{
						m_dynamicShapeContIt = m_dynamicShapeCont.erase(m_dynamicShapeContIt);


						// ****** TODO: This below probably is not needed as above we get working iterator
						if (m_dynamicShapeContIt == m_dynamicShapeCont.end() && m_dynamicShapeCont.size() == 0)
						{
							break;
						}
					}
				}
			}
		}

		static uint32 shaderID = 0;

		void IRender::createShape(IShape** shape, const PritimiveShapeConstrInfo& cInfo)
		{
			if (m_shaderManager != nullptr)
			{
				IShader* primitiveShader = m_shaderManager->getShader(0);
			}

			// ****** NOTE: This is temp. stuff, needs to be redesigned!
			// ****** TODO: Redesign and use custom allocator!
			GLXShaderConstrInfo cShInfo
			{
				shaderID,
				"..\\Data\\Shaders\\Shape\\primitiveShape.vsh",
				"..\\Data\\Shaders\\Shape\\primitiveShape.fsh"
			};
			shaderID++;
			IShader* shader = new CGLXShader(cShInfo);

			//For each shape there will be separate material, but shaders will be shared !
			IMaterial* testMaterial = new CPrimitiveMaterial(shader);
			switch (cInfo.shapeType)
			{
			case PrimitiveShapeTypes::S_BOX:
			{
				IShape* testShape = new CBoxShape(testMaterial, cInfo.info.BoxShapeInfo.width,
					cInfo.info.BoxShapeInfo.length, cInfo.info.BoxShapeInfo.height);
				*shape = testShape;
				break;
			}
			default:
				*shape = nullptr;
				break;
			}
		}

		template<class T>
		inline T getDistSq(const glm::vec3& pos1, const glm::vec3& pos2)
		{
			return static_cast<T>((pos2.x - pos1.x) * (pos2.x - pos1.x) + 
				(pos2.y - pos1.y) * (pos2.y - pos1.y) + 
				(pos2.z - pos1.z) * (pos2.z - pos1.z));
		}

		void IRender::swapLODS()
		{
			for (auto shContIt = m_staticShapeCont.begin();
			shContIt != m_staticShapeCont.end(); ++shContIt)
			{
				float dist = getDistSq<float>((*shContIt)->getPosition(), m_camera->getCameraPos());
				float drawDist = (*shContIt)->getDrawDist();
				drawDist = drawDist * drawDist;
				if (drawDist < dist)
				{
					//Check if a mesh has lower res model (LOD)
					if ((*shContIt)->getParentLOD() != nullptr)
					{
						(*shContIt) = (*shContIt)->getParentLOD();
					}
					else if ((drawDist * DRAW_DIST_MULTIPLIER) < dist)
					{
						// ****** TODO: pop the mesh out of rendering container, and put it into the 
						//some kind of container which stores meshes potentially to be loaded, otherwise return back
						//to the manager which added the shape (world manager).
						m_unusedShapeCont.push_back((*shContIt));
						m_staticShapeCont.erase(shContIt++);
					}

				}
				else if (drawDist > dist)
				{
					//Check if a mesh has higher res model (LOD)
					if ((*shContIt)->getChildLOD() != nullptr)
					{
						//Check if child LOD is in drawing dist
						float childDrawDist = (*shContIt)->getChildLOD()->getDrawDist();
						if ((childDrawDist * childDrawDist) > dist)
						{
							(*shContIt) = (*shContIt)->getChildLOD();
						}
					}
				}
			}
		}

		// ****** TODO: Needs to be finished, all shapes are pushed back as static shapes !!!
		//Checks if unused shapes become in draw dist, if not removes them from rendering pipeline.
		void IRender::processUnusedShapes()
		{
			for (auto shContIt = m_unusedShapeCont.begin();
			shContIt != m_unusedShapeCont.end(); ++shContIt)
			{
				float dist = getDistSq<float>((*shContIt)->getPosition(), m_camera->getCameraPos());
				float drawDist = (*shContIt)->getDrawDist();
				drawDist = drawDist * drawDist;
				//Just in case if camera starts to return back, 
				//push the shape back into the one of the rendering containers
				//(currently only static, this needs to be fixed !!!).
				if (((drawDist * DRAW_DIST_MULTIPLIER) > dist))
				{
					m_staticShapeCont.push_back((*shContIt));
					m_unusedShapeCont.erase(shContIt++);
				}
				else if (((drawDist * (DRAW_DIST_MULTIPLIER + 1)) < dist))
				{
					// ****** TODO: Definitely send back shape to the world manager (and destroy GPU data)
					//or destroy shape (depending on flags ?)
					//Or maybe it is time to see if we can draw LOD Group Shape ?
				}
			}
		}

		bool sortFrontToBack(const IShape* shape1, const IShape* shape2)
		{
			//Determine how far is the shape1 and shape2 from the camera.
			float m_dist1 = shape1->getDrawDist(); //glm::distance(shape1->getPosition(), Render::getInstance().getCamera()->getCameraPos());
			float m_dist2 = shape2->getDrawDist();//glm::distance(shape2->getPosition(), Render::getInstance().getCamera()->getCameraPos());
												  //Compare calculated values and return the result.
			if (m_dist1 < m_dist2)
				return true;
			else
				return false;
		}

		bool IRender::sortBackToFront(const IShape* shape1, const IShape* shape2)
		{
			//Determine how far is the shape1 and shape2 from the camera.
			float m_dist1 = getDistSq<float>(shape1->getPosition(), m_camera->getCameraPos());
			float m_dist2 = getDistSq<float>(shape2->getPosition(), m_camera->getCameraPos());
			if (m_dist1 > m_dist2)
				return true;
			else
				return false;
		}

		bool IRender::sortFrontToBack(const IShape* shape1, const IShape* shape2)
		{
			//Determine how far is the shape1 and shape2 from the camera.
			float m_dist1 = getDistSq<float>(shape1->getPosition(), m_camera->getCameraPos());
			float m_dist2 = getDistSq<float>(shape2->getPosition(), m_camera->getCameraPos());
			if (m_dist1 < m_dist2)
				return true;
			else
				return false;
		}

		bool sortFrontToBack2(const IShape* shape1, const IShape* shape2)
		{
			//Determine how far is the shape1 and shape2 from the camera.
			float m_dist1 = getDistSq<float>(shape1->getPosition(), glm::vec3(0,0,0));
			float m_dist2 = getDistSq<float>(shape2->getPosition(), glm::vec3(0, 0, 0));
			if (m_dist1 < m_dist2)
				return true;
			else
				return false;
		}

		//This is required to because when a primitive is rasterized,
		//its z value can be used to do an "early z kill", 
		//which skips running the fragment shader.
		void IRender::sortSolidFrontToBack()
		{
#ifndef _DEBUG
			//&IRender::sortFrontToBack
			//m_staticShapeCont.sort(sortBackToFront); //sortFrontToBack2 works ... but we can't pass camera there ...
#endif
		}

		void IRender::sortTransparentBackToFront()
		{
// #ifndef _DEBUG
// 			m_transparentShapeCont.sort(sortBackToFront);
// #endif 
		}

		void IRender::processLODGroups()
		{

		}
	}

} //namespace BEngine