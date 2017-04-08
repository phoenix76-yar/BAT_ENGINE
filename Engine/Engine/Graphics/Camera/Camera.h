/***********************************************************************
CAMERA

http://www.lighthouse3d.com/tutorials/view-frustum-culling/view-frustums-shape/
http://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/
***********************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include "../GraphicsDefinitions.h"

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat4x4.hpp"

namespace BEngine
{
	namespace BGraphics
	{
		static constexpr float ZOOM_IN_MAXIMUM = 3.2f;
		static constexpr float ZOOM_OUT_MAXIMUM = 10000.0f;

		//Camera Construction Info
		struct CameraConstrInfo
		{
			float fieldOfView;
			float aspectRatio;
			float nearPlaneDistance;
			float farPlaneDistance;
		};

		enum class CameraType
		{
			ORBIT,
			FREE,
			ANIM,
			ORTHOGRAPHIC
		};

		enum class FrustmIntersectType
		{
			OUTSIDE,
			INTERSECT, 
			INSIDE
		};

		//-----------------------------------------------------------------------
		//CViewFrustum
		//
		//Camera View Frustum
		//-----------------------------------------------------------------------

		// ****** TODO: Add draw method for visualization
		class CViewFrustum
		{
		public:
			CViewFrustum() = default;
			~CViewFrustum() = default;


			glm::vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

			float nearD, farD, ratio, angle, tang;
			float nw, nh, fw, fh;

			void setCamInternals(float angle, float ratio, float nearD, float farD);
			void setCamDef(glm::vec3 &p, glm::vec3 &l, glm::vec3 &u);

		private:
			//View Frustum Corners
			glm::vec3 m_nPlaneTopLeft;
			glm::vec3 m_nPlaneTopRight;
			glm::vec3 m_nPlaneBottomRight;
			glm::vec3 m_nPlaneBottomLeft;

			glm::vec3 m_fPlaneTopLeft;
			glm::vec3 m_fPlaneTopRight;
			glm::vec3 m_fPlaneBottomLeft;
			glm::vec3 m_fPlaneBottomRight;

			float m_nPlaneWidth;
			float m_nPlaneHeight;

			float m_fPlaneWidth;
			float m_fPlaneHeight;
		};

		//-----------------------------------------------------------------------
		//ABSTRACT BASE CAMERA CLASS
		//
		//Includes basic data and methods that are needed for other methods.
		//This class must be inherited by all other camera class implementations.
		//-----------------------------------------------------------------------

		class ICamera
		{
		public:
			ICamera();
			virtual ~ICamera() = default;

			virtual void					reset() = 0;
			virtual void					initialize() = 0;
			virtual void					updateViewMatrix() = 0;

			const CameraType&				getCameraType() const { return m_cameraType; };

			void							setCameraConstrInfo(CameraConstrInfo&);
			inline const CameraConstrInfo&	getCameraConstrInfo() const;

			//Returns camera projection matrix multiplied by viewMatrix.
			glm::mat4						getViewProjectionMatrix() const; // ****** TODO: All methods that have Projection rename like this method: getViewProjMatrix()
			inline const glm::mat4&			getViewMatrix() const;
			inline const glm::mat4&			getProjectionMatrix() const;

			//This is virtual method because orthographic camera overrides this method
			//(matrix for projecting two-dimensional coordinates onto the screen  -  glm::ortho(...)).
			virtual void					updateProjectionMatrix();

			void							setAspectRatio(float);
			void							setFieldOfView(float);
			void							setNearPlaneDistance(float);
			void							setFarPlaneDistance(float);

			float							getAspectRatio() const { return m_aspectRatio; };
			float							getFieldOfView() const { return m_fieldOfView; };
			float							getNearPlaneDist() const { return m_nearPlaneDistance; };
			float							getFarPlaneDist() const { return m_farPlaneDistance; };

			glm::vec3						getCameraPos() const { return m_cameraPos; };
			glm::vec3						getTargedPos() const { return m_targedPos; };
			float							getCameraHeading() const; //Returns camera's heading based on camera's and target's positions.	
			float							getDistBetweenCameraAndTarget() const;

			virtual void					setDistanceBetweenCamAndTarget(float) {}
		protected:
			CameraType						m_cameraType;
			bool							m_zoomLock;
			float							m_fieldOfView;
			float							m_aspectRatio;
			float							m_nearPlaneDistance;
			float							m_farPlaneDistance;
			float							m_distBetweenCamAndTarget;

			glm::vec3						m_cameraPos;
			glm::vec3						m_targedPos;
			glm::vec3						m_upVec;
			glm::mat4						m_viewMatrix;
			glm::mat4						m_projectionMatrix;
		};


		//-----------------------------------------------------------------------
		//ORBIT CAMERA CLASS
		//
		//Camera that rotates around a point, camera can be moved around
		//in the 3d world, made similarly as it is in 3ds max, the only difference
		//is that this camera can not move by z axis by user input.
		//-----------------------------------------------------------------------

		class OrbitCamera : public ICamera
		{
		public:
			OrbitCamera(const CameraConstrInfo&);
			OrbitCamera(const CameraConstrInfo&,
				glm::vec3 cameraPos,
				glm::vec3 targetPos
				);

			OrbitCamera(float fieldOfView,
				float aspectRatio,
				float nearPlaneDistance,
				float farPlaneDitance,
				glm::vec3 position,
				glm::vec3 targetPos
				);
			~OrbitCamera() = default;

			void			reset() override;
			void			initialize() override;

			void			updateViewMatrix() override;
			void			updateMousePos(glm::vec2 mousePosOnScreen);
			void			moveCamera(glm::vec2 mousePos, double renderTime); // ****** NOTE: Currently renderTime is ignored (rename to time step and pass timeStep !)

			void			setDistanceBetweenCamAndTarget(float dist) override;
			void			setCameraTargetPos(glm::vec3 pos);

			void			lockZoom(bool);
			bool			isZoomLocked();
		private:
			float			m_angleHPlane;
			glm::vec2		m_mousePos;
			glm::vec2		m_previousMousePos;
			double			m_lastTime;
		};
	}
}

#endif //CAMERA_H