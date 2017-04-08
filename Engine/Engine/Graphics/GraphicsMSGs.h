#ifndef GRAPHICS_MSGS_H
#define GRAPHICS_MSGS_H

#include "../EngineDefinitions.h"
#include "GraphicsDefinitions.h"

namespace BEngine
{
	namespace BGraphics
	{
		enum class GraphicsMSGType
		{
			GR_ADD_SHAPE,
			GR_ADD_LIGHT,
		};

		class IShape;

		class CAddShape
		{
		public:
			CAddShape() = default;
			~CAddShape() = default;

			//Pointer to a shape or an entity that represents shape ?
			IShape* shape; // ****** TODO: This should be replaced by bvector<IShape*> shapeCont;
		};

		class CGraphicsMSG
		{
		public:
			CGraphicsMSG() = default;
			~CGraphicsMSG() = default;

			GraphicsMSGType msgType;
			union
			{
				CAddShape addShapeMSG;
			} graphicsMSG;
		};
	} // namespace BGraphics
} //namespace BEngine

#endif //GRAPHICS_MSGS_H