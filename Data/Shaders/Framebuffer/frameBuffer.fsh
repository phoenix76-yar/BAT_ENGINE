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
};

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture; //Each shapes normal is already converted to tangent space (multiplied by normalMatrix)

in VertexInfo
{
	vec4 vertPos1;
	vec2 vertUVCh1;
};

//out vec4 fragmentColor;
layout(location = 0) out vec4 fragmentColor;

void main(void)  
{ 
	vec4 Iamb = texture(albedoTexture, vec2(vertUVCh1.x,( 1 -vertUVCh1.y)));
	fragmentColor = Iamb;
}