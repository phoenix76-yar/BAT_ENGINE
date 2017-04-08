/***********************************************************************
AUDIOMANAGER


***********************************************************************/

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "../IManager.h"

namespace BEngine 
{
	namespace BAudio
	{
		class CAudioManager : public IManager
		{
		public:
			CAudioManager() = default;
			~CAudioManager();

		private:
			void update() override final;
 			void processInputMSGs(const bvector<CEventMSG>&) override;
 			void processAudioMSG(const CEventMSG&);
		};
	} //namespace BAudio
} //namespace BEngine
#endif //AUDIOMANAGER_H


