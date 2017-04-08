/***********************************************************************
TRIANGLE SHAPE

General Triangle shape, for specific stuff for example grass instancing
implement custom classes.

If you get ugly looking shape, and triangles are connecting the wrong
vertices's, that could mean that the shape doesn't have a uv layout applied.
***********************************************************************/

#ifndef TRIANGLESHAPE_H
#define TRIANGLESHAPE_H

#include "../IShape.h"
#include "TriangleShapeDefinitions.h"

namespace BEngine 
{
	namespace BGraphics
	{
		static constexpr uint32 TRI_SHAPE_DATA_BUFFER_SIZE = 64 * 4;

		class BG1GLXBuffer;
		class BG1GLXVertexArray;
		class IMaterial;

		struct TriShapeConstrInfo
		{
			const char* fileName;
		};

		struct TriShapeInstancedData
		{
			const void* dataBuffer;
			size_t dataBuffersize;
		};

		class CTriangleShape : public IShape
		{
		public:
			CTriangleShape() = default;
			CTriangleShape(const TriShapeConstrInfo&);
			virtual ~CTriangleShape();

			virtual void update(BGRenderInfo&) override;
			virtual void draw(BGRenderInfo&) override;
			virtual void initialize(BGRenderInfo&) override;
			virtual void destroyGPUData(BGRenderInfo&) override;

		protected:
			void processShapeData(size_t bufferSize, byte* buffer);
			void initTriShapeData(BGRenderInfo&);
			void initInstancedBuffer(BGRenderInfo&);
			void unbindBufferObjects(BGRenderInfo&);

		private:
			TriShapeFileHeader64*	m_fileHeader;
			Vertex48*				m_vertexArray;
			SubShape*				m_subShapes;
			uchar*					m_perInstanceData;

			IMaterial*				m_material;

			//GPU related data
			BG1GLXVertexArray*		m_vertArrBuffer;
			BG1GLXBuffer*			m_vertexBuffer;
			BG1GLXBuffer*			m_indexBuffer;
			BG1GLXBuffer*			m_instancedDataBuffer;
			BG1GLXBuffer*			m_uniformShapeData;
			uchar					m_uniformRawShapeData[TRI_SHAPE_DATA_BUFFER_SIZE];
		};


		//-----------------------------------------------------------------------
		//CInstanced Triangle Shape
		//
		//The same triangle shape just with instanced buffer.
		//See if you can inherit from Triangle Shape class,
		//
		//-----------------------------------------------------------------------

		class CInstancedTriangleShape
		{

		};
		

	} //namespace BGraphics
} //namespace BEngine
#endif //TRIANGLESHAPE_H
