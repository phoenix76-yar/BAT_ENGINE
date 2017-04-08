/***********************************************************************
BUFFER OBJECTS

Wrapped openGL buffer objects into custom classes.
***********************************************************************/

#ifndef GLX_BUFFER_OBJECTS_H
#define GLX_BUFFER_OBJECTS_H

#include "../GraphicsDefinitions.h"

namespace BEngine
{
	namespace BGraphics
	{
		enum class BG1BufferType
		{
			VERTEX_ARRAY_BUFFER,
			INDEX_ELEMENT_ARRAY_BUFFER,
			CONSTANT_UNIFORM_BUFFER
		};

		enum class BG1BufferFormat
		{
			FORMAT_INT8,
			FORMAT_INT16,
			FORMAT_INT32,
			FORMAT_INT64,
			FORMAT_UINT8,
			FORMAT_UINT16,
			FORMAT_UINT32,
			FORMAT_UINT64,
			FORMAT_FLOAT16,
			FORMAT_FLOAT32,
			FORMAT_FLOAT64
		};

		enum class BGDrawType
		{
			B_STATIC_DRAW,
			B_DYNAMIC_DRAW
		};

		class IRender;
		//More flags to be added, such as, 
		//dynamic data, static data, etc.
		struct BG1BufferConstrInfo // ****** TODO: Rename to BufferConstrInfo
		{
			const void* dataBuffer;
			size_t dataBufferSize; // ****** NOTE: the size of the buffer must be constant
			IRender* renderPtr;
			BG1BufferType bufferType;
			uint32 shaderLayoutLoc; //Shader Layout Location for a buffer "layout (binding = 0)"
		};


		//-----------------------------------------------------------------------
		//OpenGLX Buffer Wrapper Class
		//-----------------------------------------------------------------------

		class BG1GLXBuffer
		{
		public:
			BG1GLXBuffer(BG1BufferConstrInfo& cInfo);
			~BG1GLXBuffer();

			void				bindForDraw(uint32 stride = 0, uint32 offset = 0);

			void				bind();
			void				unbind();

			//You can only map or unmap constant - uniform buffer
			void				mapBuffer(const void* dataBuffer, size_t dataSize);
			void				unmapBuffer();

			void				bufferSubData(BG1BufferType type, int offset, size_t dataSize, const void* data);
		protected:
			BG1BufferConstrInfo	m_constrInfo; // ****** TODO: Rename to m_buffConstrInfo;
			const void*			m_bufferData;
			uint32				m_bufferID;
		};

		//-----------------------------------------------------------------------
		//OpenGLX Vertex Array Wrapper Class
		//-----------------------------------------------------------------------

		class BG1GLXVertexArray
		{
		public:
			BG1GLXVertexArray();
			~BG1GLXVertexArray();

			void	bind();
			void	unbind();

			void	setVertexAttribPointer(uint32 index,
				uint32 size,
				uint32 type,
				bool normalized,
				size_t stride,
				const void* pointer
				);
			void	setVertexAttribDivisor(uint32 index, uint32 divisor);
			void	enableVertexAttribArray(uint32 index);

			void	applyVertexAttribInputLayout(uint32 numOfElements, VertexInputElement* vElements);
			void	applyInstancedAttribInputLayout(uint32 numOfElements, InstanceInputElement* iElements);
		protected:
			uint32	m_vertexArrayID;
		};

	} //BGraphics
} //BEngine

#endif //GLX_BUFFER_OBJECTS_H