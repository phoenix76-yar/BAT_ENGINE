#include "BMotionState.h"

namespace BEngine
{
	namespace BPhysics
	{
		void CMotionState::getWorldTransform(btTransform & centerOfMassWorldTrans) const
		{
			centerOfMassWorldTrans = m_graphicsWorldTrans * m_centerOfMassOffset.inverse();
		}

		void CMotionState::setWorldTransform(const btTransform & centerOfMassWorldTrans)
		{
			m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset;
		}
	}//BPhysics
} //namespace BEngine