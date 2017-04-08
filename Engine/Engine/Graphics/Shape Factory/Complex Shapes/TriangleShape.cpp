#include "TriangleShape.h"

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
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

		CTriangleShape::CTriangleShape(const TriShapeConstrInfo& cInfo)
		{
			m_shapeIsInit = false;
			FILE* fIn = nullptr;
			fopen_s(&fIn, cInfo.fileName, "rb");
			if (fIn != nullptr)
			{
				fseek(fIn, 0, SEEK_END);
				auto m_shapeDataBufferSize = ftell(fIn);
				fseek(fIn, 0, SEEK_SET);
				uchar* m_shapeDataBuffer = new uchar[m_shapeDataBufferSize];
				fread(m_shapeDataBuffer, 1, m_shapeDataBufferSize, fIn);
				this->processShapeData(m_shapeDataBufferSize, m_shapeDataBuffer);
				fclose(fIn);
			}
		}

		CTriangleShape::~CTriangleShape()
		{
			if (m_subShapes != nullptr) { delete[] m_subShapes; }
		}

		void CTriangleShape::update(BGRenderInfo& dInfo)
		{
			if (m_shapeIsInit)
			{
				if (dInfo.cameraPtr)
				{
					glm::mat4 MVP = dInfo.cameraPtr->getViewProjectionMatrix() * m_shapeWorldPosMatrix;
					glm::mat4 normalMatrix = glm::inverseTranspose(dInfo.cameraPtr->getViewMatrix() * m_shapeWorldPosMatrix);

					memcpy(&m_uniformRawShapeData[0], &m_shapeWorldPosMatrix[0][0], 64);
					memcpy(&m_uniformRawShapeData[64], &MVP[0][0], 64);
					memcpy(&m_uniformRawShapeData[128], &normalMatrix[0][0], 64);

					//Don't forget that there are additional 16 floats that can be used for additional shape - model data.
					//In this data we need to store, uv offsets (we will be able to animate uv's), uv scale ? 

					//uniformShapeData->mapBuffer(&m_shapeWorldPosMatrix[0][0], 64);
					m_uniformShapeData->mapBuffer(m_uniformRawShapeData, TRI_SHAPE_DATA_BUFFER_SIZE);
					m_uniformShapeData->unmapBuffer();
				}
			}
		}

		void CTriangleShape::draw(BGRenderInfo& rInfo)
		{
			if (m_shapeIsInit)
			{
				m_uniformShapeData->bindForDraw(TRI_SHAPE_DATA_BUFFER_SIZE, 0);
				//Always bind shader before draw!
				m_material->bind(rInfo); // ****** TODO: Bind the first material [0] (for the first shape)
				m_vertArrBuffer->bind();

				int offsetVal = 0;
				for (int sb = 0; sb < m_fileHeader->subShapeCount; sb++)
				{
					//Bind material here
					//m_material->bind();, material class should technically determine to switch shader or not to switch
					rInfo.renderPtr->drawElements(BG1PrimitiveTopology::BG_TRIANGLES, m_subShapes[sb].indiciesCount, BUFFER_OFFSET(m_subShapes[sb].bufferOffset));
					offsetVal += m_subShapes[sb].indiciesCount * sizeof(float);
				}

				m_vertArrBuffer->unbind();
				m_material->unbind(rInfo);
				m_uniformShapeData->unbind();
			}
		}

		void CTriangleShape::initialize(BGRenderInfo& rInfo)
		{
			this->initTriShapeData(rInfo);
			//this->initInstancedBuffer(rInfo);
			this->unbindBufferObjects(rInfo);
		}

		void CTriangleShape::destroyGPUData(BGRenderInfo& rInfo)
		{
			delete m_vertexBuffer;
			delete m_indexBuffer;
			delete m_vertArrBuffer;
			delete m_uniformShapeData;

			if (m_instancedDataBuffer)
				delete m_instancedDataBuffer;
		}

		void CTriangleShape::processShapeData(size_t bufferSize, byte* buffer)
		{
			uchar* dataBuffer = (uchar*)buffer;
			m_fileHeader = (TriShapeFileHeader64*)(dataBuffer + 0);
			if (m_fileHeader->signature.isTriShape() && m_fileHeader->majorVersion == 0x31 && m_fileHeader->minorVersion == 0x32)
			{
				// ****** TODO: Check vertex format
				m_vertexArray = (Vertex48*)(dataBuffer + sizeof(TriShapeFileHeader64));
				uchar* subShapePtr = dataBuffer + m_fileHeader->offsetToIndexData; 
				uchar* boneDataPtr = dataBuffer + m_fileHeader->offsetToBoneData;


				//Read Index Data
				m_subShapes = new SubShape[m_fileHeader->subShapeCount];
				for (uint32 i = 0; i < m_fileHeader->subShapeCount; i++)
				{
					m_subShapes[i].indiciesCount = *(uint32*)(subShapePtr);
					m_subShapes[i].indiciesCount *= 3;
					subShapePtr += 4;
					m_subShapes[i].materialID = *(uint32*)(subShapePtr);
					subShapePtr += 4;
					m_subShapes[i].indiciesArray = (uint32*)(subShapePtr);
					subShapePtr += m_subShapes[i].indiciesCount * sizeof(uint32);
				}

				//All other stuff goes here (bone definitions/structures, attachment points, materials, etc.)
			}
		}

		void CTriangleShape::initTriShapeData(BGRenderInfo& rInfo)
		{
			if (!m_shapeIsInit)
			{
				BG1BufferConstrInfo vertBuffCInfo
				{
					&m_vertexArray[0],
					sizeof(Vertex48) * m_fileHeader->vertexCount,
					nullptr,
					BG1BufferType::VERTEX_ARRAY_BUFFER
				};

				//Here we will allocate just an empty buffer where we will latter upload our index buffer data.
				BG1BufferConstrInfo indexBuffCInfo
				{
					nullptr,
					sizeof(uint32) * m_fileHeader->totalIndexCount * 3,
					nullptr,
					BG1BufferType::INDEX_ELEMENT_ARRAY_BUFFER
				};

				BG1BufferConstrInfo uniformShapeDataBuffCInfo
				{
					NULL,
					TRI_SHAPE_DATA_BUFFER_SIZE,
					nullptr,
					BG1BufferType::CONSTANT_UNIFORM_BUFFER,
					1 //Binding location point in the shader.
				};

				//For each vertex type define custom shader input layout

				//As defined in the shader layout (12 * sizeof(float) = this is the total buffer size per vertex)
				uint32 numberOfElements = 4;
				VertexInputElement inputElements[] =
				{
					{ "VertexPos", 0, BG_FLOAT32, false, 3 , 12 * sizeof(float), 0 },
					{ "VertexNormal", 1, BG_FLOAT32, true, 3 , 12 * sizeof(float), (void*)(sizeof(float) * 3) },
					{ "VertexTangent", 2, BG_FLOAT32, true, 4 , 12 * sizeof(float), (void*)(sizeof(float) * 6) },
					{ "VertexUV", 3, BG_FLOAT32, false, 2 , 12 * sizeof(float), (void*)(sizeof(float) * 10) }
				};

				if (m_material != nullptr)
				{
					m_material->getShader()->bind();

					// ****** TODO: We can't do buffer = new buffer;, due to memory allocation, therefore we will need to return 
					//to the render methods for creating arrays, buffers, etc.
					m_vertArrBuffer = new BG1GLXVertexArray;
					m_vertArrBuffer->bind();
					m_vertexBuffer = new BG1GLXBuffer(vertBuffCInfo);
					m_indexBuffer = new BG1GLXBuffer(indexBuffCInfo);
					m_uniformShapeData = new BG1GLXBuffer(uniformShapeDataBuffCInfo);

					m_vertArrBuffer->applyVertexAttribInputLayout(numberOfElements, inputElements);

					int offsetVal = 0;
					for (uint32 sb = 0; sb < m_fileHeader->subShapeCount; sb++)
					{
						m_indexBuffer->bufferSubData(BG1BufferType::INDEX_ELEMENT_ARRAY_BUFFER, offsetVal,
							m_subShapes[sb].indiciesCount * sizeof(int), m_subShapes[sb].indiciesArray);
						m_subShapes[sb].bufferOffset = offsetVal;
						offsetVal += m_subShapes[sb].indiciesCount * sizeof(int);
					}

					m_shapeIsInit = true;
				}
				// ****** TODO: Needs to be finished!
			}
		}

		void CTriangleShape::initInstancedBuffer(BGRenderInfo& rInfo)
		{
			//To use instanced buffers (but don't forget that
			//you also need to create instanced data buffer.
			uint32 numberOfInstInputElements = 1;
			InstanceInputElement instanceInputElemenets[] =
			{
			 	{ 4, BG_FLOAT16, false, 4 * sizeof(float16), sizeof(float16), 0},
			};
			 
			uint32 instanceInputElementsSize = 4 * 2;
			uint32 instanceElementCount = 1;

			BG1BufferConstrInfo instcBuffCInfo
			{
				&m_perInstanceData[0], //&m_instPlacementData[0]  
				instanceInputElementsSize * instanceElementCount,
				nullptr,
				BG1BufferType::VERTEX_ARRAY_BUFFER
			};


			//And then apply:
			 m_vertArrBuffer->applyInstancedAttribInputLayout(numberOfInstInputElements, instanceInputElemenets);



			 //Can we create a second Vertex Array object for storing instanced position data ? Seems that not!
		}

		void CTriangleShape::unbindBufferObjects(BGRenderInfo& rInfo)
		{
			m_vertArrBuffer->unbind();
			m_vertexBuffer->unbind();
			m_indexBuffer->unbind();
			m_uniformShapeData->unbind();

			if (m_instancedDataBuffer) m_instancedDataBuffer->unbind();

		}
	} //namespace BGraphics 
} //namespace BEngine