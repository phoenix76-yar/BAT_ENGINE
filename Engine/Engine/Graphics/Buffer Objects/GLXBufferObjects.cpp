#include "GLXBufferObjects.h"

#include "glew.h"

namespace BEngine
{
	namespace BGraphics
	{
		//-----------------------------------------------------------------------
		//OpenGLX Buffer Wrapper Class Methods
		//-----------------------------------------------------------------------

		BG1GLXBuffer::BG1GLXBuffer(BG1BufferConstrInfo & cInfo)
		{
			m_constrInfo = cInfo;
			//m_renderHandle = cInfo.renderPtr;
			m_bufferData = cInfo.dataBuffer;
			switch (cInfo.bufferType)
			{
			case BG1BufferType::VERTEX_ARRAY_BUFFER:
			{
				glGenBuffers(1, &m_bufferID);
				glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
				glBufferData(GL_ARRAY_BUFFER, cInfo.dataBufferSize, cInfo.dataBuffer, GL_STATIC_DRAW);
				break;
			}
			case BG1BufferType::INDEX_ELEMENT_ARRAY_BUFFER:
			{
				glGenBuffers(1, &m_bufferID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, cInfo.dataBufferSize, cInfo.dataBuffer, GL_STATIC_DRAW);
				break;
			}
			case BG1BufferType::CONSTANT_UNIFORM_BUFFER:
			{
				//See GL Uniform variables as buffer objects
				glGenBuffers(1, &m_bufferID);
				glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
				glBufferData(GL_UNIFORM_BUFFER, cInfo.dataBufferSize, cInfo.dataBuffer, GL_DYNAMIC_DRAW);
				//Index  (currently 0 ) is the position/location in the shader program defined by the "layout (binding = 0)", 
				//similar thing applies to the DX11 (registers), somehow it will be necessary to pass this info via buffer construction structure.
				glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_bufferID);
				break;
			}
			default:
				break;
			}
		}

		BG1GLXBuffer::~BG1GLXBuffer()
		{
			glDeleteBuffers(1, &m_bufferID);
		}

		void BG1GLXBuffer::bindForDraw(uint32 stride, uint32 offset)
		{
			switch (m_constrInfo.bufferType)
			{
				// 	case BG1BufferType::VERTEX_ARRAY_BUFFER:
				// 	{
				// 		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
				// 		break;
				// 	}
				// 	case BG1BufferType::INDEX_ELEMENT_ARRAY_BUFFER:
				// 	{
				// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
				// 		break;
				// 	}
			case BG1BufferType::CONSTANT_UNIFORM_BUFFER:
			{
				glBindBufferBase(GL_UNIFORM_BUFFER, m_constrInfo.shaderLayoutLoc, m_bufferID);
				//glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
				break;
			}
			default:
				break;
			}
		}

		void BG1GLXBuffer::bind()
		{
			switch (m_constrInfo.bufferType)
			{
				// 	case BG1BufferType::VERTEX_ARRAY_BUFFER:
				// 	{
				// 		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
				// 		break;
				// 	}
				// 	case BG1BufferType::INDEX_ELEMENT_ARRAY_BUFFER:
				// 	{
				// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
				// 		break;
				// 	}
			case BG1BufferType::CONSTANT_UNIFORM_BUFFER:
			{
				//glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_bufferID);
				glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
				break;
			}
			default:
				break;
			}
		}

		void BG1GLXBuffer::unbind()
		{
			switch (m_constrInfo.bufferType)
			{
			case BG1BufferType::CONSTANT_UNIFORM_BUFFER:
			{
				glBindBuffer(GL_UNIFORM_BUFFER, 0);
				break;
			}
			default:
				break;
			}
		}

		void BG1GLXBuffer::mapBuffer(const void* data, size_t dataSize)
		{
			switch (m_constrInfo.bufferType)
			{
			case BG1BufferType::CONSTANT_UNIFORM_BUFFER:
			{
				glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
				void* tempPtr = (void*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
				memcpy(tempPtr, data, dataSize);
				break;
			}
			default:
				break;
			}
		}

		void BG1GLXBuffer::unmapBuffer()
		{
			switch (m_constrInfo.bufferType)
			{
			case BG1BufferType::CONSTANT_UNIFORM_BUFFER:
			{
				//glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);  // ****** NOTE: This must be controlled by the programmer, by using bind method.
				bool rs = glUnmapBuffer(GL_UNIFORM_BUFFER);
				break;
			}
			default:
				break;
			}
		}

		void BG1GLXBuffer::bufferSubData(BG1BufferType type, int offset, size_t dataSize, const void * data)
		{
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, dataSize, data);
		}

		//-----------------------------------------------------------------------
		//OpenGLX Vertex Array Wrapper Class Methods
		//-----------------------------------------------------------------------

		BG1GLXVertexArray::BG1GLXVertexArray() { glGenVertexArrays(1, &m_vertexArrayID); }
		BG1GLXVertexArray::~BG1GLXVertexArray() { glDeleteVertexArrays(1, &m_vertexArrayID); }

		void BG1GLXVertexArray::bind() { glBindVertexArray(m_vertexArrayID); }
		void BG1GLXVertexArray::unbind() { glBindVertexArray(0); }

		void BG1GLXVertexArray::setVertexAttribPointer(uint32 index, uint32 size, uint32 type, bool normalized, size_t stride, const void * pointer)
		{
			glVertexAttribPointer(index, size, type, normalized, stride, pointer);
		}

		void BG1GLXVertexArray::setVertexAttribDivisor(uint32 index, uint32 divisor)
		{
			glVertexAttribDivisor(index, divisor);
		}

		void BG1GLXVertexArray::enableVertexAttribArray(uint32 index)
		{
			glEnableVertexAttribArray(index);
		}

		void BG1GLXVertexArray::applyVertexAttribInputLayout(uint32 numberOfElements, VertexInputElement* vElements)
		{
			//Set Vertex Attribute Pointers
			uint32 variableFormat = GL_FLOAT;
			for (uint32 i = 0; i < numberOfElements; i++)
			{
				switch (vElements[i].format)
				{
				case BG1Format::BG_CHAR:
					variableFormat = GL_BYTE;
					break;
				case BG1Format::BG_UCHAR:
					variableFormat = GL_UNSIGNED_BYTE;
					break;
				case BG1Format::BG_INT16:
					variableFormat = GL_SHORT;
					break;
				case BG1Format::BG_UINT16:
					variableFormat = GL_UNSIGNED_SHORT;
					break;
				case BG1Format::BG_INT32:
					variableFormat = GL_INT;
					break;
				case BG1Format::BG_UINT32:
					variableFormat = GL_UNSIGNED_INT;
					break;
				case BG1Format::BG_FLOAT16:
					variableFormat = GL_HALF_FLOAT;
					break;
				case BG1Format::BG_FLOAT64:
					variableFormat = GL_DOUBLE;
					break;
				default:
					variableFormat = GL_FLOAT;
					break;
				}
				this->setVertexAttribPointer(vElements[i].attributeID, vElements[i].size, variableFormat, vElements[i].normalized, vElements[i].stride, vElements[i].pointer);
				this->enableVertexAttribArray(vElements[i].attributeID);
			}

// 			//Enable Vertex Attribute Arrays
// 			for (uint32 i = 0; i < numberOfElements; i++)
// 			{
// 				this->enableVertexAttribArray(vElements[i].attributeID);
// 			}
		}

		void BG1GLXVertexArray::applyInstancedAttribInputLayout(uint32 numberOfElements, InstanceInputElement* iElements)
		{
			uint32 variableFormat = GL_FLOAT;
			for (uint32 i = 0; i < numberOfElements; i++)
			{
				switch (iElements[i].format)
				{
				case BG1Format::BG_CHAR:
					variableFormat = GL_BYTE;
					break;
				case BG1Format::BG_UCHAR:
					variableFormat = GL_UNSIGNED_BYTE;
					break;
				case BG1Format::BG_INT16:
					variableFormat = GL_SHORT;
					break;
				case BG1Format::BG_UINT16:
					variableFormat = GL_UNSIGNED_SHORT;
					break;
				case BG1Format::BG_INT32:
					variableFormat = GL_INT;
					break;
				case BG1Format::BG_UINT32:
					variableFormat = GL_UNSIGNED_INT;
					break;
				case BG1Format::BG_FLOAT16:
					variableFormat = GL_HALF_FLOAT;
					break;
				case BG1Format::BG_FLOAT64:
					variableFormat = GL_DOUBLE;
					break;
				default:
					variableFormat = GL_FLOAT;
					break;
				}
				this->setVertexAttribPointer(iElements[i].attributeID, iElements[i].size,
					variableFormat, iElements[i].normalized, iElements[i].stride, iElements[i].pointer);
				this->enableVertexAttribArray(iElements[i].attributeID);
				this->setVertexAttribDivisor(iElements[i].attributeID, iElements[i].divisor);
			}

// 			//Enable Vertex Attribute Arrays
// 			for (uint32 i = 0; i < numberOfElements; i++)
// 			{
// 				this->enableVertexAttribArray(iElements[i].attributeID);
// 			}
// 
// 			//Set Vertex Attribute Divisor
// 			for (uint32 i = 0; i < numberOfElements; i++)
// 			{
// 				this->setVertexAttribDivisor(iElements[i].attributeID, iElements[i].divisor);
// 			}
		}

	} //BGraphics
} //BEngine