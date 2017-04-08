#version 430

layout (location = 0) in vec4 vertPos;
layout (location = 1) in vec2 vertUV1;

layout (binding = 0) uniform CameraData
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	mat4 CameraProps;
	mat4 Padding;	
};

layout (binding = 1) uniform ShapeData
{
	mat4 ModelMatrix;
	mat4 MVPMatrix;
};

out VertexInfo
{
	vec4 vertPos1;
	vec2 vertUVCh1;
};

void main() 
{	
	vertPos1 = vertPos;
	vertUVCh1 = vertUV1;
	gl_Position = vertPos;
}