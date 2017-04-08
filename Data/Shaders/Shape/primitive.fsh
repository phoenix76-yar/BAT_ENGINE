#version 430

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
	mat4 NormalMatrix;
	mat4 PaddingMatrix; //Will be used later
};

in VertexInfo
{
	vec3 vertPos1;
	vec4 vertCol1;
	mat4 ModelViewMatrix;
};

//out vec4 fragmentColor; 
layout(location = 0) out vec4 fragmentColor; //This works only on openGL => 3.3
layout(location = 1) out vec4 posLoc;
layout(location = 2) out vec4 normalColor;

const vec3 lpos = vec3(-25,20,50); //light position

void main(void)  
{ 
	posLoc = vec4(vertPos1, 1.0f);
	//final r colour = vertCol1.x / 255;
	
	fragmentColor = vertCol1;
	normalColor = vec4(0.0, 0.0, 0.0, 1.0f);
}