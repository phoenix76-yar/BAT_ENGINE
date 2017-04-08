#include "BCollision.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btTriangleShape.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.h"
#include "BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btMaterial.h"
#include "BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h"

namespace BEngine
{
	namespace BPhysics
	{

		// ****** NOTE: Single Material col mesh
// 		m_indexVertexArray = new btTriangleIndexVertexArray(
// 			m_indiciesCount, m_indicies, 3 * sizeof(int),
// 			m_vertexCount, m_vertices, 3 * sizeof(float));
// 		//It is better to use "useQuantizedAabbCompression=true"
// 		m_colShape = new btBvhTriangleMeshShape(m_indexVertexArray, true);


		// ****** NOTE: Multimaterial col mesh
//materials count will be + 1 as in actual variable is 1 if there are 2 materials ! as arrays start from 0 !
// 		m_indexVertexArray = new btTriangleIndexVertexMaterialArray(
// 			m_indiciesCount, m_indicies, 3 * sizeof(int),
// 			m_vertexCount, m_vertices, 3 * sizeof(float),
// 			m_materialsCount + 1, (unsigned char *)m_colMaterials, sizeof(FSColMaterial),
// 			m_matIndicies, sizeof(int));
// 
// 		m_colShape = new btMultimaterialTriangleMeshShape(
// 			(btTriangleIndexVertexMaterialArray*)m_indexVertexArray, true);//fsTrimeshShape;

	} //namespace BPhysics
} //namespace BEngine