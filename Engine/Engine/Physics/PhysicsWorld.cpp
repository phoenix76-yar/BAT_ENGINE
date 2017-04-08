#include "PhysicsWorld.h"

#include "btBulletDynamicsCommon.h"

extern ContactAddedCallback gContactAddedCallback;

namespace BEngine
{
	namespace BPhysics
	{
		CPhysicsWorld::CPhysicsWorld()
		{

		}

		CPhysicsWorld::~CPhysicsWorld()
		{

		}

		static constexpr float MAX_FRICTION = 10.0f;

		inline float calculateCombinedFriction(float friction0, float friction1) 
		{
			//std::cout << friction0 << " : " << friction1 << std::endl;
			float friction = friction0 * friction1;
			
			if (friction < -MAX_FRICTION)
				friction = -MAX_FRICTION;
			if (friction > MAX_FRICTION)
				friction = MAX_FRICTION;
			return friction;
		}

		inline float calculateCombinedRestitution(float restitution0, float restitution1) 
		{
			float f_restitution = restitution0 * restitution1;
			if (f_restitution < 0) {
				return 0;
			}
			else {
				return f_restitution;
			}
		}

		bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
			const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
		{
			//btManifoldPoint //is the collision/impact point !
			// Apply material properties
// 			if (colObj0Wrap->getCollisionShape()->getShapeType() == TRIANGLE_SHAPE_PROXYTYPE) {
// 				const btCollisionShape* parent0 = colObj0Wrap->getCollisionObject()->getCollisionShape();
// 				if (parent0 != 0 && parent0->getShapeType() == MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE) {
// 					btMultimaterialTriangleMeshShape* shape = (btMultimaterialTriangleMeshShape*)parent0;
// 
// 					//const btMaterial* props = shape->getMaterialProperties(partId1, index1);
// 
// 					const FSColMaterial* props = (FSColMaterial*)shape->getMaterialProperties(partId1, index1);
// 
// 					cp.m_combinedFriction = calculateCombinedFriction(props->m_friction, colObj0Wrap->getCollisionObject()->getFriction());
// 					cp.m_combinedRestitution = calculateCombinedRestitution(props->m_restitution, colObj0Wrap->getCollisionObject()->getRestitution());
// 					//cp.m_appliedImpulse
// 				}
// 			}
// 			else if (colObj1Wrap->getCollisionShape()->getShapeType() == TRIANGLE_SHAPE_PROXYTYPE) {
// 				const btCollisionShape* parent1 = colObj1Wrap->getCollisionObject()->getCollisionShape();
// 				if (parent1 != 0 && parent1->getShapeType() == MULTIMATERIAL_TRIANGLE_MESH_PROXYTYPE)
// 				{
// 					btMultimaterialTriangleMeshShape* shape = (btMultimaterialTriangleMeshShape*)parent1;
// 
// 					//const btMaterial* props = shape->getMaterialProperties(partId1, index1);
// 					const FSColMaterial* props = (FSColMaterial*)shape->getMaterialProperties(partId1, index1);
// 
// 					cp.m_combinedFriction = calculateCombinedFriction(props->m_friction, colObj0Wrap->getCollisionObject()->getFriction());
// 					cp.m_combinedRestitution = calculateCombinedRestitution(props->m_restitution, colObj0Wrap->getCollisionObject()->getRestitution());
// 				}
// 			}
// 			return true;
			return false;
		}

		void CPhysicsWorld::initialize()
		{
			if (!m_isInitialized)
			{			
				gContactAddedCallback = collisionCallback;

				m_broadphase = new btDbvtBroadphase();

				m_collisionConfiguration = new btDefaultCollisionConfiguration;
				m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
				m_solver = new btSequentialImpulseConstraintSolver;


				
				//There are 5 more types of world see: enum btDynamicsWorldType
				//btContinuousDynamicsWorld ignoore it not completed.
				//btDiscreteDynamicsWorld - Simplest physics world, only handles rigid bodies, maybe it has better performance.
				//btSoftRigidDynamicsWorld - The only physics world that can work with large jello moulds
				//btContinuousDynamicsWorld - If you have really fast objects this will prevent them from prenetrating 
				//each other or flying through each other, but is otherwise like a btDiscreteDynamicsWorld.
				//btGpuDynamicsWorld ???

				//If you want to use soft bodies, use btSoftRigidDynamicsWorld, otherwise use btDiscreteDynamicsWorld .

				m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);


			http://bulletphysics.org/mediawiki-1.5.8/index.php/BtContactSolverInfo
				btContactSolverInfo& info = m_dynamicsWorld->getSolverInfo();
				//info.m_restingContactRestitutionThreshold = 1e30;
				//info.m_numIterations = 10;
				//info.m_solverMode = SOLVER_FRICTION_SEPARATE;

				m_dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, -9.807f));
				auto t = m_dynamicsWorld->getWorldType();
				m_isInitialized = true;
				m_state = BPhysicsState::RUNNING;
			}
		}

		void CPhysicsWorld::shutDown()
		{
			if (m_isInitialized)
			{
				//cleanUpRigidBodies();
				delete m_dynamicsWorld;
				delete m_solver;
				delete m_collisionConfiguration;
				delete m_dispatcher;
				delete m_broadphase;
				m_isInitialized = false;
			}
		}

		void CPhysicsWorld::pauseSimulation()
		{

		}

		void CPhysicsWorld::update(float fps)
		{
			if (m_state == BPhysicsState::RUNNING) 
			{
				m_dynamicsWorld->stepSimulation(1 / fps, 10);
			}
			//m_dynamicsWorld->stepSimulation(1 / fps, 10);
		}

		void CPhysicsWorld::addRigitBody(BRigidBody * rBody)
		{

		}

		void CPhysicsWorld::addRigidBody(BRigidBody * rBody, short group, short mask)
		{

		}

		void CPhysicsWorld::removeRigidBody(BRigidBody * rBody)
		{

		}

		void CPhysicsWorld::addCollision(BCollision * bCol)
		{

		}

		void CPhysicsWorld::removeCollision(BCollision * bCol)
		{

		}

		void CPhysicsWorld::addConstraint(btTypedConstraint *, bool disableCollisionsBetweenLinkedBodies)
		{

		}

		void CPhysicsWorld::removeConstraint(btTypedConstraint *)
		{

		}	
	} //Namespace BPhysics		
} //namespace BEngine