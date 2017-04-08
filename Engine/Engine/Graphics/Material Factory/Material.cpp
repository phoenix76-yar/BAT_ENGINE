#include "Material.h"
#include "../Shader Factory/IShader.h"

namespace BEngine
{
	namespace BGraphics
	{

		//-----------------------------------------------------------------------
		//CPrimitiveMaterial Class Methods
		//-----------------------------------------------------------------------

		void CPrimitiveMaterial::bind(BGRenderInfo& rInfo)
		{
			if (rInfo.activeShaderPtr != m_materialShader)
			{
				m_materialShader->bind();
				rInfo.activeShaderPtr = m_materialShader;
			}
			
		}

		void CPrimitiveMaterial::unbind(BGRenderInfo& rInfo)
		{
			//m_materialShader->unbind();
		}

	} //BGraphics
} //BEngine