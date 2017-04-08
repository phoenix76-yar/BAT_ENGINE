/***********************************************************************
GRAPHICS DEFINITIONS

All graphics definitions are located here.
***********************************************************************/

#ifndef GRAPHICS_DEFINITIONS_H
#define GRAPHICS_DEFINITIONS_H

#include "../EngineDefinitions.h"
#include "BGVertexFormats.h"

namespace BEngine
{
	namespace BGraphics
	{
		enum BShaderTypes
		{
			SHADER_PRIMITIVE = 4,
			SHADER_PHONG_SIMPLE,
			SHADER_PHONG_INSTANCED = 8,

		};

		enum class PrimitiveShapeTypes
		{
			S_BOX
		};

		struct PritimiveShapeConstrInfo
		{
			BShaderTypes shaderType;
			PrimitiveShapeTypes shapeType;
			union
			{
				struct
				{
					float width, length, height;
				} BoxShapeInfo;
			} info;
		};

		enum class BGRenderAPIType
		{
			NONE,
			GLX_RENDER,
			DX11_RENDER
		};

		struct BGRenderConstrInfo
		{
			void* windowHandle;
			uint32 width;
			uint32 height;
			bool isFullScreen;
			bool isVsyncEnabled;
		};

		class IRender;
		class ICamera;
		class IShader;

		struct BGRenderInfo
		{
			IRender* renderPtr;
			ICamera* cameraPtr;
			IShader* activeShaderPtr;
			uint32 activeShaderProgID; //Or better pass activeShaderPtr or use activeMaterialPtr ?
		};

		enum class BG1PrimitiveTopology // ****** TODO: Rename to BGPrimTopology
		{
			BG_LINES,
			BG_TRIANGLES,
			BG_TRIANGLE_STRIP
		};

		//In shader pass as 4 half floats (8 bytes), 
		//last half float split into two bytes inside a shader,
		//using bitwise operators to gather rotZ and scale.
		struct GrassInstancedPlacement8
		{
			// ****** NOTE: Those positions are local to the main mesh
			hfloat x;
			hfloat y;
			hfloat z;
			byte rotZ; //Rotation around Z axis.
			byte scaleFactor;
		};
	} //BGraphics
} //BEngine

#endif //GRAPHICS_DEFINITIONS_H