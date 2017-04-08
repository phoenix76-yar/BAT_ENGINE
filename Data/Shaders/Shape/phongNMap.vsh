#version 430

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec4 VertexTangent;
layout (location = 3) in vec2 VertexUV;

layout (binding = 0) uniform CameraData
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 CameraProps;
	mat4 Padding;
};

layout (binding = 1) uniform ShapeData
{
	mat4 ModelMatrix; //Model Transformation Matrix
	mat4 MVPMatrix;
	mat4 NormalMatrix;
	mat4 PaddingMatrix; //Will be used later
};

out VertexInfo
{
	vec3 vertPos1;
	vec3 vertNormal1;
	vec2 vertUVCh1;
	mat3 ToObjectLocal; //TBN
	mat4 ModelViewMatrix;
};

void main() 
{
	//normalMatrix = glm::inverseTranspose(dInfo.cameraPtr->getViewMatrix() * m_shapeWorldPosMatrix)
	vec3 T = normalize((NormalMatrix * vec4(VertexTangent.xyz, 1.0)).xyz);
	vec3 N = normalize((NormalMatrix * vec4(VertexNormal, 1.0)).xyz);
	
	//Binormal can be calculated on the fly: B = normalize(cross(N, T)) * VertexTangent.w;
	vec3 VertexBinormal = normalize(cross(VertexNormal, vec3(VertexTangent.xyz)) * VertexTangent.w);
	vec3 B = normalize((NormalMatrix * vec4(VertexBinormal, 1.0)).xyz);
	
	//Matrix for transformation to tangent space. (Rename to TBN Matrix ?).
	ToObjectLocal = mat3(
		T.x, B.x, N.x,
		T.y, B.y, N.y,
		T.z, B.z, N.z
	);
	
	ToObjectLocal = transpose(ToObjectLocal);
	
	ModelViewMatrix = ViewMatrix * ModelMatrix; //In instanced rendering use this place to calculate MVMatrix, just replace ModelMatrix with instanced matrix

	vertPos1 = VertexPos;
	vertNormal1 = VertexNormal;
	vertUVCh1 = VertexUV;
	gl_Position = (MVPMatrix * vec4(VertexPos, 1.0f));
}