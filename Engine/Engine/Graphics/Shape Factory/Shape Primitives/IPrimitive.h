#ifndef IPRIMITIVE_H
#define IPRIMITIVE_H

#include "../IShape.h"
#include "../../GraphicsDefinitions.h"

namespace BEngine
{
	namespace BGraphics
	{
		static constexpr uint32 PRIMITIVE_SHAPE_DATA_BUFFER_SIZE = 64 * 4;

		class BG1GLXBuffer;
		class BG1GLXVertexArray;
		class IMaterial;

		//-----------------------------------------------------------------------
		//Base Primitive Class
		//
		//Wrapper class on top of the IShape class that contains some common variables
		//and methods that are used by primitive shapes.
		//-----------------------------------------------------------------------

		class IPrimitive : public IShape
		{
		public:
			IPrimitive() = default;
			virtual ~IPrimitive() = default;

			virtual void		update(BGRenderInfo&) override;
			virtual void		draw(BGRenderInfo&) override;
			virtual void		initialize(BGRenderInfo&) override = 0;
			virtual void		destroyGPUData(BGRenderInfo&) override = 0;

		protected:
			//Primitive material
			IMaterial*			m_material;

			//Geometry variables
			uint32				vertexCount;
			uint32				indexCount;

			//Common buffer objects
			BG1GLXVertexArray*	vertArrBuffer;
			BG1GLXBuffer*		vertexBuffer;
			BG1GLXBuffer*		indexBuffer;
			BG1GLXBuffer*		uniformShapeData;
			uchar				uniformRawShapeData[PRIMITIVE_SHAPE_DATA_BUFFER_SIZE];
		};
	} //BGraphics
} //BEngine
#endif //IPRIMITIVE_H