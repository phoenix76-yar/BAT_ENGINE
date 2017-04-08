/***********************************************************************
Material

You need to set material to a shape, so the renderer would draw it.
In this file are defined the most common materials, to define your own,
extend MaterialType enum and inherit IMaterial class as a base class.
***********************************************************************/

#ifndef GRAPHICS_MATERIAL_H
#define GRAPHICS_MATERIAL_H

#include "../GraphicsDefinitions.h"
#include "../Shader Factory/IShader.h"

namespace BEngine
{
	namespace BGraphics
	{
		class ITexture;

		enum class MaterialType
		{
			MAT_PRIMITIVE,
			MAT_PHONG,
			MAT_PBR_METALINESS
		};

		//-----------------------------------------------------------------------
		//IMaterial
		//
		//Base material class, that must be inherited by all other material class
		//implementations.
		//-----------------------------------------------------------------------

		class IMaterial
		{
		public:
			IMaterial(IShader* shader, const MaterialType mType) 
			{
				m_materialShader = shader;
				m_materialType = mType; 
			}

			virtual ~IMaterial() = default;

			IShader* getShader() const { return m_materialShader; }

			virtual void bind(BGRenderInfo&) = 0;
			virtual void unbind(BGRenderInfo&) = 0;

		protected:
			IShader* m_materialShader;

		private:
			MaterialType m_materialType;
		};

		//-----------------------------------------------------------------------
		//Primitive Material
		//
		//Material used to render/draw primitive shapes.
		// ****** TODO: Add more variables like RGB colour, etc.
		//-----------------------------------------------------------------------

		class CPrimitiveMaterial : public IMaterial
		{
		public:
			CPrimitiveMaterial(IShader* shader) : IMaterial(shader, MaterialType::MAT_PRIMITIVE) {}
			virtual ~CPrimitiveMaterial() = default;

			void bind(BGRenderInfo&) override final;
			void unbind(BGRenderInfo&) override final;
		};


	} //BGraphics
} //BEngine

#endif //GRAPHICS_MATERIAL_H