/***********************************************************************
ISHAPE

Base abstract class for all shapes.

Update, draw and initialize methods must be only called from renderer!

We also need to add flag, if a shape can be destroyed by the IRender or it must be forwarded back
to the manager which originally requested to render it.

But the graphics data GPU VRAM data must be destroyed by the IRender !!!, otherwise
due to the asynchronous calls of delete buffers the engine will crash.

Non of the methods is thread safe (only those who return const data)!

***********************************************************************/

#ifndef ISHAPE_H
#define ISHAPE_H

#include "../../EngineDefinitions.h"
#include "../GraphicsDefinitions.h"

//Math classes
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat4x4.hpp"
#include "gtc/quaternion.hpp"

namespace BEngine
{
	namespace BGraphics
	{
		class CLODGroup;
		typedef const glm::mat4 cmat4;

		union BaseFlags
		{
			BaseFlags() { value = 0; }
			struct
			{
				bool shapeIsDynamic : 1;
				bool shapeIsTransparent : 1;
				bool shapeIsFoliage : 1; //Shape represents foliage (grass, trees, etc)
				bool shapeIsLODGroup : 1; //Shape represents LOD Group
			} field;
			uint16 value;
		};

		class IShape
		{
			friend class GLXRender; //Or whatever class that will call draw(ShapeDrawInfo&) method
			friend class Minimap;
		public:
			IShape();
			virtual ~IShape() = default;

			//The following 4 methods must be called only by Render class!
			//Update method is the place where to update constant/uniform shader data.
			virtual void		update(BGRenderInfo&) = 0;
			virtual void		draw(BGRenderInfo&) = 0;
			virtual void		initialize(BGRenderInfo&) = 0;
			virtual void		destroyGPUData(BGRenderInfo&) = 0;

			void				setPosition(glm::vec3);
			void				setRotationByQuat(glm::quat); // ****** NOTE: IMPORTANT GLM TAKES QUATERNION AS W X Y Z !!!
			void				setRotationByAngle(glm::vec3 rotation, float angle);
			void				setScale(glm::vec3);
			void				setTransformation(const glm::mat4&);

			glm::vec3			getPosition() const;
			glm::quat			getRotationAsQuat() const;
			glm::vec3			getScale() const;
			// ****** TODO: change to: inline const glm::mat4&	getShapeTransformation() const;
			inline cmat4&		getShapeTransformation() const;

			void				setParentLOD(IShape* lodShape) { m_parentLOD = lodShape; }
			void				setChildLOD(IShape* lodShape) { m_childLOD = lodShape; }
			void				setLODGroup(CLODGroup* lodGroup) { m_lodGroup = lodGroup; }

			IShape*				getParentLOD() const { return m_parentLOD; }
			IShape*				getChildLOD() const { return m_childLOD; }
			CLODGroup*			getLODGroup() const { return m_lodGroup; }

			void				setDrawDist(float dist) { m_drawDist = dist; }
			float				getDrawDist() const { return m_drawDist; }

			void				setDrawShapeFlag(bool flag) { m_drawShape = flag; }
			bool				getDrawShapeFlag() const { return m_drawShape; }
			const BoundBox&		getBoundBox() const { return m_boundBox; }

			inline void			setFlags(uint16 flags);
			inline void			setFlags(BaseFlags flags);
			BaseFlags			getFlags() const { return m_flags; }
		protected:
			//Every single shape must have defined bounding box (with some very rare exceptions such as grid),
			//as later it will be used for clicking and selection methods.
			BoundBox			m_boundBox;

			CLODGroup*			m_lodGroup;

			IShape*				m_parentLOD;
			IShape*				m_childLOD;
			
			//Transformation/position data
			glm::vec3			m_position;
			glm::quat			m_rotation;
			glm::vec3			m_scale;
			glm::mat4			m_shapeWorldPosMatrix; //Shape Model Matrix

			float				m_drawDist;
			bool				m_shapeIsInit;
			bool				m_drawShape; //By default true (used for culling)
			BaseFlags			m_flags;
		};
		
	} //BGraphics
} //BEngine

#endif //ISHAPE_H
