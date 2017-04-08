#include "LODGroup.h"
#include "IShape.h"

namespace BEngine
{
	namespace BGraphics
	{
		CLODGroup::~CLODGroup()
		{

		}

		void CLODGroup::updateLODGroupShape(BGRenderInfo& rInfo)
		{
			if (m_LODGroupShape != nullptr)
			{
				m_LODGroupShape->update(rInfo);
			}
		}

		void CLODGroup::drawLODGroupShape(BGRenderInfo& rInfo)
		{
			if (m_LODGroupShape != nullptr) 
			{
				m_LODGroupShape->update(rInfo);
			}
		}

		void CLODGroup::removeLODShape(IShape* shape)
		{
			for (auto shContIt = m_LODShapes.begin();
			shContIt != m_LODShapes.end(); ++shContIt)
			{
				if (shape == (*shContIt))
				{
					shContIt = m_LODShapes.erase(shContIt);
// 					if (shContIt == m_childShapes.end() && m_childShapes.size() == 0)
// 					{
// 						break;
// 					}
				}
			}
		}


	} //namespace BGraphics
} //namespace BEngine