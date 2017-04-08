/***********************************************************************
CBCOLLISION


***********************************************************************/

#ifndef CBCOLLISION_H
#define CBCOLLISION_H

#include "../EngineDefinitions.h"

class btCollisionShape;
class btTriangleIndexVertexArray;

namespace BEngine 
{
	namespace BPhysics
	{
		class CBColMaterial;

		class CBCollision
		{
		public:
			CBCollision() = default;
			~CBCollision() = default;

		private:
			uint32 m_vertexCount;
			uint32 m_indexCount;

			float* m_vertices;
			uint32*	m_indicies;
			uint32* m_matIndicies;

			CBColMaterial* m_colMaterials;

			btCollisionShape* m_colShape;
			btTriangleIndexVertexArray*	 m_indexVertexArray;
		};
	} //namespace BPhysics
	
} //namespace BEngine
#endif //CBCOLLISION_H

