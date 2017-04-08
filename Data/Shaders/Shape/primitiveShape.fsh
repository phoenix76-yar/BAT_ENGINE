//https://learnopengl.com/#!Lighting/Basic-Lighting

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
	vec3 vertNormal1;
	vec4 vertCol1;
	mat4 ModelViewMatrix;
};

//out vec4 fragmentColor; 
layout(location = 0) out vec4 fragmentColor; //This works only on openGL => 3.3
layout(location = 1) out vec4 posLoc;
layout(location = 2) out vec4 normalColor;

const vec3 lpos = vec3(5,5,20); //light position

void main(void)  
{ 

	vec3 v = (ModelMatrix * vec4(vertPos1, 1.0)).xyz; //vertPos1; //(ModelViewMatrix * vec4(vertPos1, 1.0)).xyz;//
	vec3 N = vertNormal1;
	vec4 diffuse = vec4(0.2, 0.4, 0.5, 1.0f);
	vec4 specular = vec4(0.2, 0.4, 0.5, 1.0f);
	float shininess = 1.0f;
	vec3 sceneColor = vec3(0.5, 0.6, 1.0f);
	vec4 ambient = vec4(0.2, 0.4, 0.5, 1.0f);

	posLoc = vec4(vertPos1, 1.0f);
	//final r colour = vertCol1.x / 255;
		
	vec3 L = normalize(lpos - v);  //normalize(((ViewMatrix * vec4(lpos,1)).xyz - v)); //
	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N));  
 
	//calculate Ambient Term:  
	vec4 Iamb = ambient;    

	//calculate Diffuse Term:  
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);     
   
	// calculate Specular Term:
	vec4 Ispec = specular * pow(max(dot(R,E),0.0),0.3*shininess );
	Ispec = clamp(Ispec, 0.0, 1.0); 
	// write Total Color:  
	fragmentColor = Iamb + Idiff + Ispec; //sceneColor + Iamb + Idiff + Ispec;

	fragmentColor = vec4(fragmentColor.xyz, 0.5);
	
	normalColor = vec4(0.0, 0.0, 0.0, 1.0f);
}