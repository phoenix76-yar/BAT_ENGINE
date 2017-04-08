/***********************************************************************
CRENDERMANAGER


***********************************************************************/

#ifndef CRENDERMANAGER_H
#define CRENDERMANAGER_H

#include "../IManager.h"
#include "GraphicsDefinitions.h"

namespace BEngine 
{
	namespace BGraphics
	{
		class IRender;
	
		class CRenderManager : public IManager
		{
		public:
			CRenderManager(const BGRenderConstrInfo& cInfo);		
			~CRenderManager();

		private:
			void update() override final;
			void processInputMSGs(const bvector<CEventMSG>&);
			void processUserInputMSG(const CEventMSG&);
			void processGraphicsMSG(const CEventMSG&);
			
		private:
			BGRenderConstrInfo m_renderCInfo;
			IRender* m_render;
			bool m_pauseRendering;
			bool m_xKeyDown;
			//IGUI* m_gui; IGUI
		};
	} //namespace BGrahpics
} //namespace BEngine
#endif //CRENDERMANAGER_H
