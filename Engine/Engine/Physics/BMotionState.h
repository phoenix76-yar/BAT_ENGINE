/***********************************************************************
MOTION STATE

http://bulletphysics.org/mediawiki-1.5.8/index.php/MotionStates
***********************************************************************/

#ifndef BMOTIONSTATE_H
#define BMOTIONSTATE_H

#include "btBulletDynamicsCommon.h"

namespace BEngine 
{
	namespace BPhysics
	{
		class CMotionState : public btMotionState
		{
		public:
			CMotionState() = default;
			virtual ~CMotionState() = default;

			virtual void getWorldTransform(btTransform& centerOfMassWorldTrans) const override;
			//Bullet only calls the update of world transform for active (dynamic) objects.
			virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override;
		private:
			btTransform	m_graphicsWorldTrans;
			btTransform	m_centerOfMassOffset;
			btTransform	m_startWorldTrans;

			btVector3 m_rigidBodyPos;
			btQuaternion m_rigidBodyRotationQuat;

			// 			FSRigidBody*	m_rigidBody;
			// 			FSBaseShape*	m_shape;
			// 			FSCamera*		m_camera;
		};

// 		class CKinematicMotionState : public btMotionState
// 		{
// 		public:
// 			CKinematicMotionState() = default;
// 			virtual ~CKinematicMotionState() = default;
// 
// 			virtual void getWorldTransform(btTransform& centerOfMassWorldTrans) const override;
// 			virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans) override;
// 		private:
// 		};

	} //BPhysics
} //namespace BEngine
#endif //BMOTIONSTATE_H
