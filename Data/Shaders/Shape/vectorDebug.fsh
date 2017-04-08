//https://www.youtube.com/watch?v=rJoAMHjT5HA
//http://fabiensanglard.net/bumpMapping/

#version 430

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

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;

in vec4 vertexColor;

//out vec4 fragmentColor;
layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;

const vec3 lpos = vec3(-10,10,20); //light position

void main(void)  
{ 
	fragmentColor = vertexColor;
	normalColor = vec4(vertexColor.xyz, 1.0f);
}