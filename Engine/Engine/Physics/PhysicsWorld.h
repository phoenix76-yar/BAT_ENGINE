/***********************************************************************
PHYSICS WORLD

C:\Users\akaln\Documents\Visual Studio 2015\Projects\GameWindow2\GameWindow2\FSEngine\FSPhysicsWorld

http://www.bulletphysics.org/Bullet/phpBB3/viewtopic.php?f=9&t=4863




if you want to ignoore some collisions (some objects don't collide with other objects use group and mask)
http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Filtering#Filtering_collisions_using_masks


Locking one of the axis for rigidvbodies, and other usefull info:
http://bulletphysics.org/mediawiki-1.5.8/index.php/Code_Snippets

***********************************************************************/

#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H

#include "../EngineDefinitions.h"
#include "../EngineContainers.h"
#include "PhysicsDefinitions.h"

#include <thread>
#include <atomic>

class btTypedConstraint;

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace BEngine 
{
	namespace BPhysics
	{
		class BRigidBody;
		class BCollision;

		enum class BPhysicsState
		{
			PAUSED,
			RUNNING
		};

		class CPhysicsWorld
		{
		public:
			CPhysicsWorld();
			~CPhysicsWorld();

			void initialize();
			void shutDown();
			void pauseSimulation();
			bool isPaused() const { return m_isPaused; }
			void update(float fps = 60.0f);

		public: // ****** NOTE: Add and Remove methods
// 			void addStaticRigidBody(BRigidBody* rBody);
// 			void addStaticRigidBody(BRigidBody* rBody, short group, short mask);
// 			void removeStaticRigidBody(BRigidBody* rBody);

			void addRigitBody(BRigidBody* rBody);
			void addRigidBody(BRigidBody* rBody, short group, short mask);
			void removeRigidBody(BRigidBody* rBody);

			void addCollision(BCollision* bCol);
			void removeCollision(BCollision* bCol);

			void addConstraint(btTypedConstraint*, bool disableCollisionsBetweenLinkedBodies = false);
			void removeConstraint(btTypedConstraint*);

			void setState(BPhysicsState state) { m_state = state; }
			BPhysicsState getState() const { return m_state; }

		private:
			BPhysicsState m_state;

			bool m_isInitialized;
			bool m_isPaused;

			btBroadphaseInterface*					m_broadphase;
			btDefaultCollisionConfiguration*		m_collisionConfiguration;
			btCollisionDispatcher*					m_dispatcher;
			btSequentialImpulseConstraintSolver*	m_solver;
			btDiscreteDynamicsWorld*				m_dynamicsWorld;

			blist<BRigidBody*>				m_staticRBodyList;
			blist<BRigidBody*>::iterator	m_staticRBodyListIt;
		};
	} //Namespace BPhysics	
} //namespace BEngine
#endif //PHYSICSWORLD_H
