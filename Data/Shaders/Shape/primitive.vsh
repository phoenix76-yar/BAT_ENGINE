#version 430

layout (location = 0) in vec3 VertexPos;
layout (location = 1) in vec4 VertexCol;

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
	vec4 vertCol1;
	mat4 ModelViewMatrix;
};

void main() 
{
	ModelViewMatrix = ViewMatrix * ModelMatrix; //In instanced rendering use this place to calculate MVMatrix, just replace ModelMatrix with instanced matrix

	vertPos1 = VertexPos;
	vertCol1 = vec4(VertexCol.x / 255, VertexCol.y / 255, VertexCol.z / 255, 1.0); //VertexCol;
	gl_Position = (MVPMatrix * vec4(VertexPos, 1.0f));
}