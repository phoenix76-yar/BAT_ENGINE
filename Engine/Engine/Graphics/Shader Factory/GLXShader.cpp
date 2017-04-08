#include "GLXShader.h"
#include "glew.h"

namespace BEngine
{
	namespace BGraphics
	{
		CGLXShader::CGLXShader(const GLXShaderConstrInfo& cInfo) : IShader(cInfo.shaderID)
		{
			m_vertexShaderSource = readFile(cInfo.vertexShaderSourceFile);
			m_fragmentShaderSource = readFile(cInfo.fragmentShaderSourceFile);

			m_vertShaderID = makeVertexShader(m_vertexShaderSource);
			m_fragShaderID = makeFragmentShader(m_fragmentShaderSource);
			m_shaderProgramID = makeShaderProgram(m_vertShaderID, m_fragShaderID);
			// ****** TODO: Use to validate the shader program
			//glValidateProgram
		}

		CGLXShader::~CGLXShader()
		{
			glDeleteProgram(m_shaderProgramID);
			glDeleteShader(m_vertShaderID);
			glDeleteShader(m_fragShaderID);

			glDetachShader(m_shaderProgramID, m_vertShaderID);
			glDetachShader(m_shaderProgramID, m_fragShaderID);
		}

		void CGLXShader::bind()
		{
			glUseProgram(m_shaderProgramID);
		}

		void CGLXShader::unbind()
		{
			glUseProgram(0);
		}

		int CGLXShader::getAttributeLocation(const char * attributeName) const
		{
			return (glGetAttribLocation(m_shaderProgramID, attributeName));
		}

		int CGLXShader::getUniformLocation(const char * uniformName) const
		{
			return (glGetUniformLocation(m_shaderProgramID, uniformName));
		}

		uint32 CGLXShader::makeVertexShader(const char * shaderSource) const
		{
			uint32 vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShaderID, 1, (const char**)&shaderSource, NULL);
			glCompileShader(vertexShaderID);
			bool compiledCorrectly = compiledStatus(vertexShaderID);
			if (compiledCorrectly)
			{
				return vertexShaderID;
			}
			return -1;
		}

		uint32 CGLXShader::makeFragmentShader(const char * shaderSource) const
		{
			uint32 fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShaderID, 1, (const char**)&shaderSource, NULL);
			glCompileShader(fragmentShaderID);
			bool compiledCorrectly = compiledStatus(fragmentShaderID);
			if (compiledCorrectly)
			{
				return fragmentShaderID;
			}
			return -1;
		}

		uint32 CGLXShader::makeGeometryShader(const char * shaderSource) const
		{
			uint32 geometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometryShaderID, 1, (const char**)&shaderSource, NULL);
			glCompileShader(geometryShaderID);
			bool compiledCorrectly = compiledStatus(geometryShaderID);
			if (compiledCorrectly)
			{
				return geometryShaderID;
			}
			return -1;
		}

		uint32 CGLXShader::makeShaderProgram(uint32 vertShaderID, uint32 fragShaderID) const
		{
			uint32 shaderID = glCreateProgram();
			glAttachShader(shaderID, vertShaderID);
			glAttachShader(shaderID, fragShaderID);
			glLinkProgram(shaderID);
			return shaderID;
		}

		uint32 CGLXShader::makeShaderProgram(uint32 vertShaderID, uint32 fragShaderID, uint32 geomShaderID) const
		{
			uint32 shaderID = glCreateProgram();
			glAttachShader(shaderID, vertShaderID);
			glAttachShader(shaderID, geomShaderID);
			glAttachShader(shaderID, fragShaderID);
			glLinkProgram(shaderID);
			return shaderID;
		}

		bool CGLXShader::compiledStatus(uint32 shaderID) const
		{
			int compiled = 0;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
			if (compiled)
			{
				return true;
			}
			else
			{
				int logLength;
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
				char* msgBuffer = new char[logLength];
				glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
#ifdef _DEBUG
				printf("----------------------\nShader Compile Error\n%s\n", msgBuffer);
#else
				// ****** TODO: LOG::getInstance().writeInfo(LOG::ERROR, msgBuffer);
#endif
				return false;
			}
		}

		char* CGLXShader::readFile(const char* fileName)
		{
			FILE* shaderFile;
			errno_t err;
			err = fopen_s(&shaderFile, fileName, "r");
			if (err == 0)
			{
				//Move the file pointer to the end of the file and determine the length
				fseek(shaderFile, 0, SEEK_END);
				long fileLength = ftell(shaderFile);
				fseek(shaderFile, 0, SEEK_SET);
				char* contents = new char[fileLength + 1];
				//Zero out memory
				for (int i = 0; i < fileLength + 1; i++)
				{
					contents[i] = 0;
				}
				//Here's the actual read
				fread(contents, 1, fileLength, shaderFile);
				//This is how you denote the end of a string in C
				contents[fileLength] = '\0';
				fclose(shaderFile);
				return contents;
			}
			else
			{
				return "EMPTY";
			}
		}
	} //BGraphics
} //namespace BEngine