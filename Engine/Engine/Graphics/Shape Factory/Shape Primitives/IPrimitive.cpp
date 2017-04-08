#include "IPrimitive.h"

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
		void IPrimitive::update(BGRenderInfo& rInfo)
		{
			if (m_shapeIsInit)
			{
				if (rInfo.cameraPtr)
				{
					glm::mat4 MVP = rInfo.cameraPtr->getViewProjectionMatrix() * m_shapeWorldPosMatrix;
					glm::mat4 normalMatrix = glm::inverseTranspose(rInfo.cameraPtr->getViewMatrix() * m_shapeWorldPosMatrix);

					memcpy(&uniformRawShapeData[0], &m_shapeWorldPosMatrix[0][0], 64);
					memcpy(&uniformRawShapeData[64], &MVP[0][0], 64);
					memcpy(&uniformRawShapeData[128], &normalMatrix[0][0], 64);

					//Don't forget that there are additional 16 floats that can be used for additional shape - model data.
					//memcpy(&uniformRawShapeData[128 + 64], &normalMatrix[0][0], 64);
					//uniformShapeData->mapBuffer(&m_shapeWorldPosMatrix[0][0], 64);
					uniformShapeData->mapBuffer(uniformRawShapeData, PRIMITIVE_SHAPE_DATA_BUFFER_SIZE);
					uniformShapeData->unmapBuffer();
				}
			}
		}

		void IPrimitive::draw(BGRenderInfo& rInfo)
		{
			if (m_shapeIsInit)
			{
				uniformShapeData->bindForDraw(PRIMITIVE_SHAPE_DATA_BUFFER_SIZE, 0);
				//Always bind shader before draw!
				m_material->bind(rInfo);
				vertArrBuffer->bind();

				rInfo.renderPtr->drawElements(BG1PrimitiveTopology::BG_TRIANGLES, indexCount);

				vertArrBuffer->unbind();
				m_material->unbind(rInfo);
				uniformShapeData->unbind();
			}
		}
	} //BGraphics
} //BEngine