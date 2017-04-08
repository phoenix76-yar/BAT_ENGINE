#ifndef BG_VERTEX_FORMATS
#define BG_VERTEX_FORMATS

#include "GraphicsDefinitions.h"

namespace BEngine
{
	namespace BGraphics
	{
		//-----------------------------------------------------------------------
		//Vector type templates
		//
		//In vector names G stands for Graphical - Graphical Vector/Container
		//-----------------------------------------------------------------------

		template<class T>
		struct BGVector2
		{
			BGVector2(T x_, T y_) : x(x_), y(y_) {}
			T x;
			T y;
		};

		template<class T>
		struct BGVector3
		{
			BGVector3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
			T x;
			T y;
			T z;
		};

		template<class T>
		struct BGVector4
		{
			BGVector4(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_) {}
			T x;
			T y;
			T z;
			T w;
		};

		//-----------------------------------------------------------------------
		//Vector2 types
		//
		//Vector2 by default is defined as a float type.
		//-----------------------------------------------------------------------

		using Vector2 = BGVector2<float>;
		using Vector2f = BGVector2<float>;
		using Vector2i = BGVector2<int>;
		using Vector2b = BGVector2<uchar>;

		//-----------------------------------------------------------------------
		//Vector3 types
		//
		//Vector3 by default is defined as a float type.
		//-----------------------------------------------------------------------

		using Vector3 = BGVector3<float>;
		using Vector3f = BGVector3<float>;
		using Vector3i = BGVector3<int>;
		using Vector3b = BGVector3<uchar>;

		//-----------------------------------------------------------------------
		//Vector4 types
		//
		//Vector4 by default is defined as a float type.
		//-----------------------------------------------------------------------

		using Vector4 = BGVector4<float>;
		using Vector4f = BGVector4<float>;
		using Vector4i = BGVector4<int>;
		using Vector4b = BGVector4<uchar>;


	

		// ****** NOTE: IF YOU UPDATE THIS ENUM, YOU MUST ALSO UPDATE applyVertexAttribInputLayout(...)
		//METHOD IN BufferObjects.cpp FILE !!!
		//https://www.khronos.org/opengl/wiki/OpenGL_Type 
		enum BG1Format // ****** TODO: Rename to BG1DataFormat
		{
			BG_CHAR,
			BG_UCHAR,  //Byte
			BG_INT16,  //Short
			BG_UINT16, //Unsigned Short
			BG_INT32,
			BG_UINT32,
			BG_FLOAT16, //Half float
			BG_FLOAT32, //Float
			BG_FLOAT64  //Double
		};

		enum BG1VertexFormat : uchar
		{
			BG_VERTEX_12 = 12,
			BG_VERTEX_16 = 16,
			BG_VERTEX_20 = 20,
			BG_VERTEX_36 = 36,
			BG_VERTEX_48 = 48,
			BG_VERTEX_52 = 52,
			BG_VERTEX_56 = 56,
			BG_VERTEX_50 = 50
		};

		struct VertexInputElement
		{
			char* semanticName; //(Name for the element inside shader)
			uint32 attributeID; //AttributeID in the shader layout for openGL
			BG1Format format;
			bool normalized;
			uint32 size;
			uint32 stride;
			const void* pointer;
		};

		struct InstanceInputElement
		{
			uint32 attributeID;
			BG1Format format;
			bool normalized;
			uint32 size;
			uint32 stride;
			uint32 divisor;
			const void* pointer;
		};

		struct Face12
		{
			Face12(uint32 id1, uint32 id2, uint32 id3) : 
				index1(id1), index2(id2), index3(id3) {}
			uint32 index1;
			uint32 index2;
			uint32 index3;
		};

		//-----------------------------------------------------------------------
		//Vertex types
		//
		//Currently non optimized structure version
		//All possible vertex types used by the graphics framework.
		//Vertex definition:
		//Vertex<number indicating how many byte structure it is>
		//All vertex structures could inherit the base vertex structure that would
		//include only position data, this way it would be easier to work with shape data.
		//
		//Also structures for vertex input layouts are needed !
		//
		//And pay attention how you define structures, for example:
		//We can't write like this:
		//struct SomeStruct { uchar x; uchar y; float z; } ;
		//end expect it to be 6 floats long, there will be additional two uchar's
		//defined after you chars the structure will be 8 bytes long !
		//it can be rewriten as: SomeStruct {uchar x; uchar u; uchar padding[2]; float z;};
		//-----------------------------------------------------------------------


// 		struct IVertex12
// 		{
// 			Vector3f pos;
// 		};

		struct Vertex16
		{
			Vector3f pos;
			Vector4b colour;
		};

		struct Vertex20
		{
			Vector3f pos;
			Vector2f uv;
		};

		struct Vertex24
		{
			Vector3f pos;
			Vector2f uv;
			Vector4b colour;
		};

		struct Vertex36
		{
			Vector3f pos;
			Vector3f normal;
			Vector2f uv;
		};

		struct Vertex48 //: public IVertex12
		{
			Vector3f pos;
			Vector3f normal;
			Vector4f tangent;
			Vector2f uv;
		};

		//We don't need second tangent vector because it is per vertex - per pixel.
		struct Vertex56
		{
			Vector3f pos;
			Vector3f normal;
			Vector4f tangent;
			Vector2f uvCh1;
			Vector2f uvCh2;
		};

		// ****** NOTE: This vertex structure to be used only for terrain,
		//it allows 6 layer texture blend: 1 base texture, 4 paint able textures, 1 second uv channel texture.
		struct Vertex60 // ****** TODO: Rename to TerrainVertex60 ?
		{
			Vector3f pos;
			Vector3f normal;
			Vector4f tangent;
			Vector4b colour; //Allows four channel texture paint
			Vector2f uvCh1;
			Vector2f uvCh2;
		};

		// ****** NOTE: To be used for skinned meshes, think how vertexBoneWeights could be optimized.
		struct Vertex68 // ****** TODO: Rename to SkinnedVertex68 ?
		{
			Vector3f pos;
			Vector3f normal;
			Vector4f tangent;
			Vector2f uv;
			Vector4b vertexBoneGroups;
			Vector4f vertexBoneWeights; //Maybe replace by shorts or half floats ?
		};
	}
}

#endif //BG_VERTEX_FORMATS
