/***********************************************************************
RIGID BODY


***********************************************************************/

#ifndef BRIGIDBODY_H
#define BRIGIDBODY_H

#include "btBulletDynamicsCommon.h"

namespace BEngine 
{
	namespace BPhysics
	{
		class CRigidBody : public btRigidBody
		{
		public:
			// ****** NOTE: All parameters such as mass, friction, etc. can be set in this construction info
			CRigidBody(btRigidBody::btRigidBodyConstructionInfo& cInfo);
			~CRigidBody();

			void setActionDist(float dist) { m_actionDist = dist; }
			float getActionDist() const { return m_actionDist; }

		private:
			float m_actionDist;

		};
	}//BPhysics
} //namespace BEngine
#endif //BRIGIDBODY_H
