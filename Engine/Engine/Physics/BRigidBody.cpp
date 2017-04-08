#include "BRigidBody.h"

namespace BEngine
{
	namespace BPhysics
	{
		CRigidBody::CRigidBody(btRigidBody::btRigidBodyConstructionInfo& cInfo) : btRigidBody(cInfo)
		{

		}

		CRigidBody::~CRigidBody()
		{
			if (this->getMotionState()) {
				delete this->getMotionState();
			}
			if (this->getCollisionShape()) {
				delete this->getCollisionShape();
			}
		}
	}//BPhysics
} //namespace BEngine