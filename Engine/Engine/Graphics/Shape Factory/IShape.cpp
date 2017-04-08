#include "IShape.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "gtx/matrix_interpolation.hpp"
#include "gtx/quaternion.hpp"
#include "gtc/matrix_inverse.hpp"

namespace BEngine
{
	namespace BGraphics
	{
		float degreesToRadians(float val) { return (float)(val * 3.141592654f / 180.0f); }

		IShape::IShape()
		{
			m_drawDist = 500.0f;
			m_drawShape = true;
		}

		void IShape::setPosition(glm::vec3 position)
		{
			m_position = position;
			m_shapeWorldPosMatrix = (glm::translate(glm::mat4(), m_position));
		}

		void IShape::setRotationByQuat(glm::quat rotation)
		{
			m_rotation = rotation;
			m_shapeWorldPosMatrix *= glm::toMat4(m_rotation);
		}

		void IShape::setRotationByAngle(glm::vec3 rotation, float angle)
		{
			m_shapeWorldPosMatrix *= glm::rotate(glm::mat4(), degreesToRadians(angle), rotation);
			m_rotation = glm::quat_cast(m_shapeWorldPosMatrix);
		}

		void IShape::setScale(glm::vec3 scale)
		{
			m_scale = scale;
			m_shapeWorldPosMatrix *= glm::scale(glm::mat4(), m_scale);
		}

		void IShape::setTransformation(const glm::mat4& tr)
		{
			m_shapeWorldPosMatrix = tr;
			m_rotation = glm::quat_cast(m_shapeWorldPosMatrix);
			m_position = glm::vec3{ m_shapeWorldPosMatrix[3][0],m_shapeWorldPosMatrix[3][1],m_shapeWorldPosMatrix[3][2] };
		}

		glm::vec3 IShape::getPosition() const { return m_position; }
		glm::quat IShape::getRotationAsQuat() const { return m_rotation; }
		glm::vec3 IShape::getScale() const { return m_scale; }
		const glm::mat4& IShape::getShapeTransformation() const { return m_shapeWorldPosMatrix; }

		void IShape::setFlags(uint16 flags)
		{
			m_flags.value = flags;
		}

		void IShape::setFlags(BaseFlags flags)
		{
			m_flags = flags;
		}
		
	} //BGraphics
} //BEngine