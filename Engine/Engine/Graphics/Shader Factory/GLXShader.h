/***********************************************************************
CGLXSHADER


***********************************************************************/

#ifndef CGLXSHADER_H
#define CGLXSHADER_H

#include "IShader.h"

namespace BEngine 
{
	namespace BGraphics
	{
		struct GLXShaderConstrInfo
		{
			uint32 shaderID;
			const char* vertexShaderSourceFile;
			const char* fragmentShaderSourceFile;
		};


		class CGLXShader : public IShader
		{
		public:
			CGLXShader(const GLXShaderConstrInfo& cInfo);
			~CGLXShader();
				
			void			bind() override final;
			void			unbind() override final;

			// ****** TODO: These function will be removed, don't utilize them!
			uint32			getShaderProgramID() const { return m_shaderProgramID; }
			int				getAttributeLocation(const char* attributeName) const;
			int				getUniformLocation(const char* uniformName) const;
		private:
			uint32			makeVertexShader(const char* shaderSource) const;
			uint32			makeFragmentShader(const char* shaderSource) const;
			uint32			makeGeometryShader(const char* shaderSource) const;
			uint32			makeShaderProgram(uint32 vertShaderID, uint32 fragShaderID) const;
			uint32			makeShaderProgram(uint32 vertShaderID, uint32 fragShaderID, uint32 geomShaderID) const;
			bool			compiledStatus(uint32 shaderID) const;
			static char*	readFile(const char* fileName);

			uint32			m_shaderProgramID;
			uint32			m_vertShaderID;
			uint32			m_fragShaderID;
			uint32			m_geomShaderID;

			char*			m_vertexShaderSource;
			char*			m_fragmentShaderSource;
			char*			m_geometryShaderSource;
		};
	} //BGraphics	
} //namespace BEngine
#endif //CGLXSHADER_H
