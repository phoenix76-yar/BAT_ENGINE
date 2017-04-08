/***********************************************************************
LOD GROUP

LOD group is technically not a shape !!!

Better re-implement it in different way.
LOD group needs to hold a shape that represents the LOD group,
and a container that holds lowest LOD shapes.
***********************************************************************/

#ifndef LODGROUPSHAPE_H
#define LODGROUPSHAPE_H

#include "../../EngineContainers.h"
#include "../GraphicsDefinitions.h"

namespace BEngine 
{
	namespace BGraphics
	{
		class IShape;

		// ****** TODO: Needs to be implemented.
		//-----------------------------------------------------------------------
		//LOD Group
		//
		//LOD Group Shape is always a static shape!
		//
		//class CLODGroup is not a shape, but an object that holds a single shape that
		//represents lower lod shapes, and a container that stores lower lod shapes.
		//-----------------------------------------------------------------------

		class CLODGroup
		{
		public:
			CLODGroup() = default;
			~CLODGroup();

			//The following 2 methods must be called by Render!
			void updateLODGroupShape(BGRenderInfo&);
			void drawLODGroupShape(BGRenderInfo&);

			void setLODGroupShape(IShape* shape) { m_LODGroupShape = shape; }
			IShape* getLODGroupShape() const { return m_LODGroupShape; }

			void addLODShape(IShape* shape) { m_LODShapes.push_back(shape); }
			void removeLODShape(IShape*);

			const blist<IShape*>& getLODShapes() const { return m_LODShapes; }
		private:
			IShape* m_LODGroupShape;
			blist<IShape*> m_LODShapes;
		};

	} //namespace BGraphics
} //namespace BEngine
#endif //LODGROUPSHAPE_H
