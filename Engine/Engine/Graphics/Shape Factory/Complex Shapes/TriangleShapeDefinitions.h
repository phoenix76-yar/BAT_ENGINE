#ifndef TRIANGLE_SHAPE_DEFS
#define TRIANGLE_SHAPE_DEFS

#include "../IShape.h"

namespace BEngine
{
	namespace BGraphics
	{
		union TriShapeSignature // ****** TODO: Rename to FSDTriShapeSignature
		{
			struct
			{
				char F;
				char S;
				char D;
				char V;
			};
			uint32 value;
			bool isTriShape() { return ((value == 'VDSF') ? true : false); } // ****** TODO: Rename to isFSDTriShape()
		};


		// ****** TODO: Add here also uchar indexFormat, it will be useful for optimization
		//they can be either 2 byte unsigned ints or 4 byte unsigned ints
		union TriShapeFlagField1
		{
			struct
			{
				BG1VertexFormat vertexFormat;
				uchar boneCount;
				bool hasMaterialData : 1;
				bool hasSkeleton : 1;
				bool hasAnimatedUVs : 1;
			} flags;
			uint32 value;
		};

		struct FSDTriShapeBone32
		{
			FSDTriShapeBone32() : parentBoneID(0), boneID(0), flags(0) {}
			uchar parentBoneID; //For Root bone always 0
			uchar boneID;
			uint16 flags; //Bone names/hashes will be saved in different place (there will be given offset in the header)

						  //28 Byte transformation data
			struct
			{
				float x;
				float y;
				float z;
			} position;

			struct
			{
				float x;
				float y;
				float z;
				float w;
			} rotation;
		};

		// ****** TODO: Needs testing, possibly will be auto-aligned to 8 bytes!!!
		struct FSDBoneName
		{
			uchar boneID;
			char* boneName;
		};

		struct TriShapeFileHeader64 // ****** TODO: Rename to FSDFileHeader64
		{
			TriShapeFileHeader64()
			{
				signature.value = 'VDSF';
				majorVersion = '1';
				minorVersion = '2';

				memset(&padding, 0, 10);

				offsetToBoneNames = 0;
				offsetToMaterialData = 0;
				offsetToIndexData = 0;
				subShapeCount = 0;
				vertexCount = 0;
				totalIndexCount = 0;
			}

			TriShapeSignature signature;
			uchar majorVersion;
			uchar minorVersion;

			uint16 subShapeCount;

			TriShapeFlagField1  flagField1;
			uchar padding[4]; //Reserved for flag field2

			uint32 offsetToBoneNames;
			uint32 offsetToMaterialData;
			uint32 offsetToIndexData; //From file start
			uint32 offsetToBoneData;

			//32 bytes
			BoundBox bounds;
			uint32 vertexCount;
			uint32 totalIndexCount;
		};

		class IMaterial;

		struct SubShape
		{
			uint32		indiciesCount;
			uint32		materialID;
			uint32*		indiciesArray; // ****** TODO: Rename to indexPtr;
			int			bufferOffset;
			IMaterial*	material; //If this causes error, include "FSMaterial.h" class into your *.cpp file.
		};

		struct ShapeBone
		{
			FSDTriShapeBone32 boneData;
			char* boneName;
		};

	} //namespace BGraphics 
} //namespace BEngine

#endif //TRIANGLE_SHAPE_DEFS
