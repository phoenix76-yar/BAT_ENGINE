#include "Camera.h"

#include <gtc/matrix_transform.hpp>

#include <glm.hpp>
#include "gtx/matrix_interpolation.hpp"
#include "gtx/quaternion.hpp"
#include "gtc/matrix_inverse.hpp"

namespace BEngine
{
	namespace BGraphics
	{
#define ANG2RAD 3.14159265358979323846/180.0

		//-----------------------------------------------------------------------
		//CViewFrustum Class Methods
		//-----------------------------------------------------------------------

		void CViewFrustum::setCamInternals(float angle, float ratio, float nearD, float farD)
		{
			// store the information
			this->ratio = ratio;
			this->angle = angle;
			this->nearD = nearD;
			this->farD = farD;

			// compute width and height of the near and far plane sections
			tang = (float)tan(ANG2RAD * angle * 0.5);
			nh = nearD * tang;
			nw = nh * ratio;
			fh = farD  * tang;
			fw = fh * ratio;
		}

		void CViewFrustum::setCamDef(glm::vec3 & p, glm::vec3 & l, glm::vec3 & u)
		{

		}





		//-----------------------------------------------------------------------
		//Base Camera Methods
		//-----------------------------------------------------------------------

		ICamera::ICamera() { m_zoomLock = false; }

		void ICamera::updateProjectionMatrix()
		{
			m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance);
		}

		glm::mat4 ICamera::getViewProjectionMatrix() const { return (getProjectionMatrix() * getViewMatrix()); }
		inline const glm::mat4& ICamera::getViewMatrix() const { return m_viewMatrix; }
		inline const glm::mat4& ICamera::getProjectionMatrix() const { return m_projectionMatrix; }

		void ICamera::setAspectRatio(float aspectRatio)
		{
			m_aspectRatio = aspectRatio;
			updateProjectionMatrix();
		}

		void ICamera::setFieldOfView(float fieldOfView)
		{
			m_fieldOfView = fieldOfView;
			updateProjectionMatrix();
		}

		void ICamera::setNearPlaneDistance(float nearPlaneDist)
		{
			m_nearPlaneDistance = nearPlaneDist;
			updateProjectionMatrix();
		}

		void ICamera::setFarPlaneDistance(float farPlaneDist)
		{
			m_farPlaneDistance = farPlaneDist;
			updateProjectionMatrix();
		}

		void ICamera::setCameraConstrInfo(CameraConstrInfo& constructInfo)
		{
			m_aspectRatio = constructInfo.aspectRatio;
			m_fieldOfView = constructInfo.fieldOfView;
			m_nearPlaneDistance = constructInfo.nearPlaneDistance;
			m_farPlaneDistance = constructInfo.farPlaneDistance;
			updateProjectionMatrix();
		}

		inline const CameraConstrInfo& ICamera::getCameraConstrInfo() const
		{
			return (CameraConstrInfo{ m_fieldOfView, m_aspectRatio, m_nearPlaneDistance, m_farPlaneDistance });
		}

		float ICamera::getDistBetweenCameraAndTarget() const
		{
			float dist = glm::distance(m_cameraPos, m_targedPos);
			return dist;
		}

		float ICamera::getCameraHeading() const
		{
			float varx = 0.0f;
			float vary = 0.0f;

			varx = m_cameraPos.x - m_targedPos.x;
			vary = m_cameraPos.y - m_targedPos.y;

			float h = std::sqrt(varx * varx + vary * vary);
			float cameraHeading = std::asin(vary / h) * 180.0f / 3.14f;

			if (varx < 0 && vary > 0) {
				cameraHeading = 180 - cameraHeading;
			}
			else if (varx < 0 && vary < 0) {
				cameraHeading = 180 + (cameraHeading * -1);
			}
			else if (varx > 0 && vary < 0) {
				cameraHeading = 360 + cameraHeading;
			}
			return cameraHeading;
		}

		//-----------------------------------------------------------------------
		//Orbit Camera Methods
		//-----------------------------------------------------------------------

		OrbitCamera::OrbitCamera(const CameraConstrInfo& cInfo)
		{
			m_cameraType = CameraType::ORBIT;
			m_fieldOfView = cInfo.fieldOfView;
			m_aspectRatio = cInfo.aspectRatio;
			m_nearPlaneDistance = cInfo.nearPlaneDistance;
			m_farPlaneDistance = cInfo.farPlaneDistance;

			//Values by default
			m_cameraPos = glm::vec3{ 20, 20, 20 };
			m_targedPos = glm::vec3{ 0,0,0 };
			m_upVec = glm::vec3{ 0, 0, 1 };
			m_mousePos = glm::vec2{ 50,50 };

			m_distBetweenCamAndTarget = getDistBetweenCameraAndTarget();
		}

		OrbitCamera::OrbitCamera(const CameraConstrInfo& cInfo, glm::vec3 cameraPos, glm::vec3 targetPos)
		{
			m_cameraType = CameraType::ORBIT;
			m_fieldOfView = cInfo.fieldOfView;
			m_aspectRatio = cInfo.aspectRatio;
			m_nearPlaneDistance = cInfo.nearPlaneDistance;
			m_farPlaneDistance = cInfo.farPlaneDistance;

			m_cameraPos = cameraPos;
			m_targedPos = targetPos;
			m_upVec = glm::vec3{ 0, 0, 1 }; //This can actually control the bank of the camera too, try: glm::vec3{0,0.5,1};
			m_mousePos = glm::vec2{ 50,50 };
			m_previousMousePos = glm::vec2{ 0.0f, 0.0f };

			m_distBetweenCamAndTarget = getDistBetweenCameraAndTarget();
		}

		OrbitCamera::OrbitCamera(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDitance, glm::vec3 cameraPos, glm::vec3 targetPos)
		{
			m_cameraType = CameraType::ORBIT;
			m_fieldOfView = fieldOfView;
			m_aspectRatio = aspectRatio;
			m_nearPlaneDistance = nearPlaneDistance;
			m_farPlaneDistance = farPlaneDitance;

			m_cameraPos = cameraPos;
			m_targedPos = targetPos;
			m_upVec = glm::vec3{ 0, 0, 1 };
			m_mousePos = glm::vec2{ 50,50 };
			m_previousMousePos = glm::vec2{ 0.0f, 0.0f };

			m_distBetweenCamAndTarget = getDistBetweenCameraAndTarget();
		}

		void OrbitCamera::lockZoom(bool flag) { m_zoomLock = flag; }
		bool OrbitCamera::isZoomLocked() { return m_zoomLock; }

		void OrbitCamera::reset()
		{
			m_cameraType = CameraType::ORBIT;
			m_cameraPos = glm::vec3{ 20, 20, 20 };
			m_targedPos = glm::vec3{ 0, 0, 0 };
			m_upVec = glm::vec3{ 0, 0, 1 };
			m_viewMatrix = glm::lookAt(m_cameraPos, m_targedPos, m_upVec);
			updateProjectionMatrix();
		}

		void OrbitCamera::initialize()
		{
			updateProjectionMatrix();
			updateViewMatrix();
		}

		// ****** TODO: MAKE A METHOD INSTEAD: cameraZoom(int delta) where delta can be either 1 or -1.
		void OrbitCamera::setDistanceBetweenCamAndTarget(float dist)
		{
			if (dist >= ZOOM_IN_MAXIMUM && dist <= ZOOM_OUT_MAXIMUM && !m_zoomLock)
			{
				m_distBetweenCamAndTarget = dist;
				updateViewMatrix();
			}
		}

		void OrbitCamera::updateMousePos(glm::vec2 mousePosOnScreen)
		{
			glm::vec2 mouseDelta = mousePosOnScreen - m_previousMousePos;
			float aDist = 50.0f;
			if (mouseDelta.x > aDist || mouseDelta.x < -aDist || mouseDelta.y > aDist || mouseDelta.y < -aDist)
			{
				mouseDelta = glm::normalize(mouseDelta);
			}
			float mPower = 1.0f;
			if (getDistBetweenCameraAndTarget() < 20)
			{
				mPower = 4.0f;
			}
			//m_mousePos.x += mouseDelta.x * mPower;
			//m_mousePos.y += mouseDelta.y * mPower;
			m_mousePos += mouseDelta * mPower;
			m_previousMousePos = mousePosOnScreen;
			updateViewMatrix();
		}

		float calcXYComponent(float x, float y)
		{
			x = abs(x);
			y = abs(y);

			float xy = std::sqrt(x*x + y*y);
			return xy;
		}

		void OrbitCamera::updateViewMatrix()
		{
			double dist = m_distBetweenCamAndTarget; // getDistBetweenCameraAndTarget(); //m_distBetweenCamAndTarget;
													 //These are camera angles !
			float rotX = m_mousePos.x / 100;
			float rotY = m_mousePos.y / 100;

			float eyeX = m_targedPos.x;
			float eyeY = m_targedPos.y;
			float eyeZ = m_targedPos.z;

			float camZ = sin(rotY) * dist;
			float camX = (sin(rotX) - (1 - cos(rotY)) * sin(rotX)) * dist;
			float camY = (cos(rotX) - (1 - cos(rotY)) * cos(rotX)) * dist;

			m_cameraPos.x = camX + eyeX;
			m_cameraPos.y = camY + eyeY;
			m_cameraPos.z = camZ + eyeZ;

			// ****** NOTE: This is the place where to apply rotation matrix for a camera !!!
			m_viewMatrix = /*glm::rotate(glm::mat4(), 1.0f / *rotation in radians* /, glm::vec3(0, 0, 1)) * */glm::lookAt(m_cameraPos, m_targedPos, m_upVec);
		}

		void OrbitCamera::setCameraTargetPos(glm::vec3 pos)
		{
			m_targedPos = pos;
			m_cameraPos = m_cameraPos + m_targedPos;
			updateViewMatrix();
		}

		void OrbitCamera::moveCamera(glm::vec2 mousePosOnScreen, double renderTime)
		{
			float fAngle = getCameraHeading() + 90.0f;

			glm::vec2 mouseDelta = mousePosOnScreen - m_previousMousePos;
			if (mouseDelta.x > 100.0f || mouseDelta.x < -100.0f || mouseDelta.y > 100.0f || mouseDelta.y < -100.0f)
			{
				mouseDelta = glm::normalize(mouseDelta);
			}

			//glm::vec2 mouseDelta = glm::normalize(mousePosOnScreen - m_previousMousePos);
			//glm::vec2 mouseDelta = mpos - previousMousePos;

			float x = getDistBetweenCameraAndTarget() / getDistBetweenCameraAndTarget();
			float deltaTime = glm::normalize(float(renderTime - m_lastTime)) * x; //1.0f;// this 0.8f to be calculated from camera distance between target and camera pos

			if (getDistBetweenCameraAndTarget() < 25.0f) { deltaTime *= 0.4f; }

			deltaTime = getDistBetweenCameraAndTarget() / 500.0f;

			//Calculate Left/Right motion
			float newX = cos((fAngle)* 3.14f / 180.0f) * mouseDelta.x * deltaTime * -1.0f;
			float newY = sin((fAngle)* 3.14f / 180.0f) * mouseDelta.x * deltaTime * -1.0f;

			m_targedPos += glm::vec3{ newX, newY, 0.0f };
			m_cameraPos += glm::vec3{ newX, newY, 0.0f };

			//Calculate Forward/Backward motion
			fAngle += 90.0f;
			// ****** TODO: Optimize use sincos(...) !!!
			newX = cos((fAngle)* 3.14f / 180.0f) * mouseDelta.y * deltaTime;
			newY = sin((fAngle)* 3.14f / 180.0f) * mouseDelta.y * deltaTime;

			m_targedPos += glm::vec3{ newX, newY, 0.0f };
			m_cameraPos += glm::vec3{ newX, newY, 0.0f };

			m_previousMousePos = mousePosOnScreen;
			m_lastTime = renderTime;
			updateViewMatrix();
		}
	
}
}