/***********************************************************************
ISHADER

Base shader class interface
***********************************************************************/

#ifndef ISHADER_H
#define ISHADER_H

#include "../GraphicsDefinitions.h"

namespace BEngine 
{
	namespace BGraphics
	{
		class IShader
		{
		public:
			IShader(uint32 shaderID) { m_shaderID = shaderID; }
			virtual ~IShader() = default;

			virtual void bind() = 0;
			virtual void unbind() = 0;

			uint32 getShaderID() const { return m_shaderID; }
		private:
			uint32 m_shaderID;
		};
	} //BGraphics
} //namespace BEngine
#endif //ISHADER_H