/***********************************************************************
SHADER MANAGER

***********************************************************************/

#ifndef CSHADERMANAGER_H
#define CSHADERMANAGER_H

#include "../../IManager.h"
#include "../GraphicsDefinitions.h"
#include "IShader.h"

namespace BEngine 
{
	namespace BGraphics
	{
		class CShaderManager : public ISimpleManager
		{
		public:
			CShaderManager() = default;
			~CShaderManager();

			void initialize() override final;
			void shutDown() override final;
			void update() override final;

			IShader* getShader(BShaderTypes type);
			IShader* getShader(uint32 shaderID);
		private:
			bvector<IShader*> m_shaders;
		};
	} //BGraphics
} //namespace BEngine
#endif //CSHADERMANAGER_H

