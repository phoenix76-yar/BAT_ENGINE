#ifndef BOX_SHAPE_H
#define BOX_SHAPE_H

#include "IPrimitive.h"

namespace BEngine
{
	namespace BGraphics
	{
		class IMaterial;

		class CBoxShape : public IPrimitive
		{
		public:
			CBoxShape(IMaterial*, float halfExtents);
			//BoxShape(IMaterial*, BoundBox bbox); // ****** TODO: To be implemented!
			CBoxShape(IMaterial*, float width, float length, float height);
			~CBoxShape();

			void initialize(BGRenderInfo&) override final;
			void destroyGPUData(BGRenderInfo&) override final;
		private:
			void constructShapeGeometry(float width, float length, float height);
					
			Vertex48* m_vertexArray;
			uint32* m_indexArray;
		};
	} //BGraphics
} //BEngine

#endif //BOX_SHAPE_H

