#version 430

//Rename these to VertexPos,
//VertexNormal, etc.

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec4 VertexTangent;
layout (location = 3) in vec2 VertexUV;

layout (binding = 0) uniform CameraData
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

layout (binding = 1) uniform ShapeData
{
	mat4 ModelMatrix;
	mat4 MVPMatrix;
	mat4 NormalMatrix;
	mat4 PaddingMatrix; //Will be used later
};

out Vertex1 //Rename to VertexInfo
{
	vec4 vertPos;
	vec4 normal;
	vec4 color;
	vec4 tangent;
	vec4 binormal;
} vertexG;

void main() 
{	
	vec3 T = normalize((NormalMatrix * vec4(VertexTangent.xyz, 1.0)).xyz);
	vec3 N = normalize((NormalMatrix * vec4(VertexNormal, 1.0)).xyz);
	
	//Binormal can be calculated on the fly: B = normalize(cross(N, T)) * VertexTangent.w;
	vec3 VertexBinormal = normalize(cross(VertexNormal, vec3(VertexTangent.xyz)) * VertexTangent.w);
	vec3 B = normalize((NormalMatrix * vec4(VertexBinormal, 1.0)).xyz);
		
	mat4 ModelViewMatrix = ViewMatrix * ModelMatrix;

	vertexG.vertPos = vec4(VertexPos, 0);
	vertexG.normal = vec4(VertexNormal, 0);
	vertexG.tangent = VertexTangent;
	vertexG.binormal = vec4(VertexBinormal, 0);
	gl_Position =  (MVPMatrix * vec4(VertexPos, 1.0f));
}