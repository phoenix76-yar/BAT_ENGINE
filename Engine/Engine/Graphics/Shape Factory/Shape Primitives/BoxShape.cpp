#include "BoxShape.h"

#include "../../Material Factory/Material.h"
#include "../../Buffer Objects/GLXBufferObjects.h"
#include "../../Camera/Camera.h"

#include "../../GLXRender.h"
#include "../../Shader Factory/GLXShader.h"

#include "glew.h"
#include "gtc/matrix_inverse.hpp"

namespace BEngine
{
	namespace BGraphics
	{
		CBoxShape::CBoxShape(IMaterial* mat, float halfExtents)
		{
			m_shapeIsInit = false;
			m_material = mat;
			this->constructShapeGeometry(halfExtents * 2, halfExtents * 2, halfExtents * 2);
		}

		CBoxShape::CBoxShape(IMaterial* mat, float width, float length, float height)
		{
			m_shapeIsInit = false;
			m_material = mat;
			this->constructShapeGeometry(width, length, height);
		}

		CBoxShape::~CBoxShape()
		{
			delete[] m_vertexArray;
			delete[] m_indexArray;
			
			BGRenderInfo emptyInfo;
			this->destroyGPUData(emptyInfo);
		}

		void CBoxShape::initialize(BGRenderInfo& rInfo)
		{
			BG1BufferConstrInfo vertBuffCInfo
			{
				&m_vertexArray[0],
				sizeof(Vertex48) * vertexCount,
				nullptr,
				BG1BufferType::VERTEX_ARRAY_BUFFER
			};

			BG1BufferConstrInfo indexBuffCInfo
			{
				&m_indexArray[0],
				sizeof(uint32) * indexCount,
				nullptr,
				BG1BufferType::INDEX_ELEMENT_ARRAY_BUFFER
			};

			BG1BufferConstrInfo uniformShapeDataBuffCInfo
			{
				NULL,
				PRIMITIVE_SHAPE_DATA_BUFFER_SIZE,
				nullptr,
				BG1BufferType::CONSTANT_UNIFORM_BUFFER,
				1
			};

			//As defined in the shader layout (12 * sizeof(float) = this is the total buffer size per vertex)
			uint32 numberOfElements = 4;
			VertexInputElement inputElements[] =
			{
				{ "VertexPos", 0, BG_FLOAT32, false, 3 , 12 * sizeof(float), 0 },
				{ "VertexNormal", 1, BG_FLOAT32, true, 3 , 12 * sizeof(float), (void*)(sizeof(float) * 3) },
				{ "VertexTangent", 2, BG_FLOAT32, true, 4 , 12 * sizeof(float), (void*)(sizeof(float) * 6) },
				{ "VertexUV", 3, BG_FLOAT32, false, 2 , 12 * sizeof(float), (void*)(sizeof(float) * 10) }
			};

			m_material->getShader()->bind();

			vertArrBuffer = new BG1GLXVertexArray;
			vertArrBuffer->bind();
			vertexBuffer = new BG1GLXBuffer(vertBuffCInfo);
			indexBuffer = new BG1GLXBuffer(indexBuffCInfo);
			uniformShapeData = new BG1GLXBuffer(uniformShapeDataBuffCInfo);

			vertArrBuffer->applyVertexAttribInputLayout(numberOfElements, inputElements);

			vertArrBuffer->unbind();
			vertexBuffer->unbind();
			indexBuffer->unbind();
			uniformShapeData->unbind();

			m_shapeIsInit = true;
		}

		void CBoxShape::destroyGPUData(BGRenderInfo& rInfo)
		{
			delete vertexBuffer;
			delete indexBuffer;
			delete vertArrBuffer;
			delete uniformShapeData;
		}

		void CBoxShape::constructShapeGeometry(float width, float length, float height)
		{
			if (!m_shapeIsInit && m_vertexArray == nullptr && m_indexArray == nullptr)
			{
				width /= 2;
				length /= 2;
				height /= 2;

				m_boundBox = BoundBox(-width, -length, -height, width, length, height);

				vertexCount = 24;
				m_vertexArray = new Vertex48[24]
				{
					Vertex48
				{
					Vector3f{ width,-length,-height },
					Vector3f{ 1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,1.0f,0.0f,1.0f },
					Vector2f{ 0.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ width,-length,height },
					Vector3f{ 1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,1.0f,0.0f,1.0f },
					Vector2f{ 0.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ width,length,-height },
					Vector3f{ 0.0f,1.0f,0.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ width,length,height },
					Vector3f{ 0.0f,1.0f,0.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ -width,length,-height },
					Vector3f{ -1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,-1.0f,0.0f,1.0f },
					Vector2f{ 0.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ -width,-length,-height },
					Vector3f{ -1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,-1.0f,0.0f,1.0f },
					Vector2f{ 1.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ -width,length,height },
					Vector3f{ -1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,-1.0f,0.0f,1.0f },
					Vector2f{ 0.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ -width,-length,height },
					Vector3f{ -1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,-1.0f,0.0f,1.0f },
					Vector2f{ 1.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ -width,-length,-height },
					Vector3f{ 0.0f,0.0f,-1.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ width,-length,-height },
					Vector3f{ 0.0f,0.0f,-1.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ -width,length,-height },
					Vector3f{ 0.0f,0.0f,-1.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ width,length,-height },
					Vector3f{ 0.0f,0.0f,-1.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ -width,-length,height },
					Vector3f{ 0.0f,0.0f,1.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ width,-length,height },
					Vector3f{ 0.0f,0.0f,1.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ -width,length,height },
					Vector3f{ 0.0f,0.0f,1.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ width,length,height },
					Vector3f{ 0.0f,0.0f,1.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ -width,-length,-height },
					Vector3f{ 0.0f,-1.0f,0.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ width,-length,-height },
					Vector3f{ 0.0f,-1.0f,0.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ -width,-length,height },
					Vector3f{ 0.0f,-1.0f,0.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 0.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ width,-length,height },
					Vector3f{ 0.0f,-1.0f,0.0f },
					Vector4f{ 1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ width,length,-height },
					Vector3f{ 1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,1.0f,0.0f,1.0f },
					Vector2f{ 1.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ width,length,height },
					Vector3f{ 1.0f,0.0f,0.0f },
					Vector4f{ 0.0f,1.0f,0.0f,1.0f },
					Vector2f{ 1.0f,0.0f }
				},
					Vertex48
				{
					Vector3f{ -width,length,-height },
					Vector3f{ 0.0f,1.0f,0.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,1.0f }
				},
					Vertex48
				{
					Vector3f{ -width,length,height },
					Vector3f{ 0.0f,1.0f,0.0f },
					Vector4f{ -1.0f,0.0f,0.0f,1.0f },
					Vector2f{ 1.0f,0.0f }
				}
				};

				indexCount = 12 * 3;
				m_indexArray = new uint32[12 * 3]
				{
					8, 10, 11,
					11, 9, 8,
					12, 13, 15,
					15, 14, 12,
					16, 17, 19,
					19, 18, 16,
					0, 20, 21,
					21, 1, 0,
					2, 22, 23,
					23, 3, 2,
					4, 5, 7,
					7, 6, 4
				};
			}
		}

		// 		void CBoxShape::update(BGRenderInfo& rInfo)
		// 		{
		// 			if (m_shapeIsInit)
		// 			{
		// 				if (rInfo.cameraPtr)
		// 				{
		// 					glm::mat4 MVP = rInfo.cameraPtr->getViewProjectionMatrix() * m_shapeWorldPosMatrix;
		// 					glm::mat4 normalMatrix = glm::inverseTranspose(rInfo.cameraPtr->getViewMatrix() * m_shapeWorldPosMatrix);
		// 
		// 					memcpy(&uniformRawShapeData[0], &m_shapeWorldPosMatrix[0][0], 64);
		// 					memcpy(&uniformRawShapeData[64], &MVP[0][0], 64);
		// 					memcpy(&uniformRawShapeData[128], &normalMatrix[0][0], 64);
		// 
		// 					//Don't forget that there are additional 16 floats that can be used for additional shape - model data.
		// 					//memcpy(&uniformRawShapeData[128 + 64], &normalMatrix[0][0], 64);
		// 					//uniformShapeData->mapBuffer(&m_shapeWorldPosMatrix[0][0], 64);
		// 					uniformShapeData->mapBuffer(uniformRawShapeData, PRIMITIVE_SHAPE_DATA_BUFFER_SIZE);
		// 					uniformShapeData->unmapBuffer();
		// 				}
		// 			}
		// 		}
		// 
		// 		void CBoxShape::draw(BGRenderInfo& rInfo)
		// 		{
		// 			if (m_shapeIsInit)
		// 			{
		// 				uniformShapeData->bindForDraw(PRIMITIVE_SHAPE_DATA_BUFFER_SIZE, 0);
		// 				//Always bind shader before draw!
		// 				m_material->bind();
		// 				vertArrBuffer->bind();
		// 
		// 				rInfo.renderPtr->drawElements(BG1PrimitiveTopology::BG_TRIANGLES, indexCount);
		// 
		// 				vertArrBuffer->unbind();
		// 				m_material->unbind();
		// 				uniformShapeData->unbind();
		// 			}
		// 		}
	} //BGraphics
} //BEngine