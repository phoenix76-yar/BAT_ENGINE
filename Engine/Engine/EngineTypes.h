/***********************************************************************
ENGINE VARIABLES

Rename to EngineTypes, and add some types here too ?
***********************************************************************/

#ifndef ENGINEVARIABLES_H
#define ENGINEVARIABLES_H

#include "Core/Half float/half.hpp"

 //using namespace half_float;
 typedef half_float::half halffloat;

namespace BEngine 
{
	typedef char				int8;
	typedef short				int16;
	typedef int					int32;
	typedef long long			int64; //Eight Byte Integer

	typedef unsigned char		uchar, byte, uint8; //Unsigned char, also known as byte.
	typedef unsigned short		uint16; //Two Byte Unsigned Integer
	typedef unsigned int		uint32; //Four Byte Unsigned Integer
	typedef unsigned long long	uint64; //Eight Byte Unsigned Integer

	typedef halffloat			float16, hfloat;

	//Shape bounding box, 24 byte structure (consists of 6 floats)
	struct BoundBox
	{
		BoundBox() : minX(0.0f), minY(0.0f), minZ(0.0f),
			maxX(0.0f), maxY(0.0f), maxZ(0.0f) {}

		BoundBox(float minX_, float minY_, float minZ_,
			float maxX_, float maxY_, float maxZ_)
		{
			minX = minX_;
			minY = minY_;
			minZ = minZ_;

			maxX = maxX_;
			maxY = maxY_;
			maxZ = maxZ_;
		}

		float minX;
		float minY;
		float minZ;
		float maxX;
		float maxY;
		float maxZ;
	};

} //namespace BEngine
#endif //ENGINEVARIABLES_H
