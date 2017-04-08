//https://www.youtube.com/watch?v=rJoAMHjT5HA
//http://fabiensanglard.net/bumpMapping/

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

//In padding matrix add, uv'offset parameters for (uv animation)

uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;

in VertexInfo
{
	vec3 vertPos1;
	vec3 vertNormal1;
	vec2 vertUVCh1;
	mat3 ToObjectLocal; //TBN
	mat4 ModelViewMatrix;
};

//out vec4 fragmentColor; 
layout(location = 0) out vec4 fragmentColor; //This works only on openGL => 3.3
layout(location = 1) out vec4 posLoc;
layout(location = 2) out vec4 normalColor;

const vec3 lpos = vec3(-25,20,50); //light position

void main(void)  
{ 
	vec3 mapNormal = texture(normalTexture, vec2(vertUVCh1.x,(vertUVCh1.y))).rgb;
	
	// Transform normal vector to range [-1,1]
	mapNormal = normalize(mapNormal * 2.0 - 1.0);
	vec3 N = ToObjectLocal * normalize(mapNormal); //normalize(normal + mapNormal);

	//vertex positin in eye coordinates (calculate this in vertex shader !)
	vec3 v = (ModelViewMatrix * vec4(vertPos1, 1.0)).xyz;	
	//light vector/light direction
	vec3 L = normalize(((ViewMatrix * vec4(lpos,1)).xyz - v)); //View matrix can be passed in deffered renderer.

	//Transform light dir. and view dir. to tangent space.
	L = normalize(L);
	//v = normalize(v); //Here only v was suppsed to be normalized
	
	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
	vec3 R = normalize(-reflect(L,N));
	
	//calculate Ambient Term:
	vec4 Iamb = texture(albedoTexture, vec2(vertUVCh1.x,(vertUVCh1.y))) * vec4(0.2, 0.4, 0.3, 1.0); //this could be the place where to add AO map. and multiply this by the light intensity like 2.5 etc.
	
	//calculate Diffuse Term:
	vec4 Idiff = vec4(0);
	Idiff = texture(albedoTexture, vec2(vertUVCh1.x,(vertUVCh1.y))) * max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);
	
	//calculate Specular Term:
	float shininess = 16.1;
	
	float lightIntensity1 = 0.7f;
	
	//calculate specular reflection only if the surface is oriented to the light source
	vec4 Ispec = vec4(0);
	if(max(dot(N, L), 0.0f) > 0) //be carefull with this, was without max(x,0.0f)
	{
		Ispec = (vec4(0.9, 0.4, 0.2, 1.0) * lightIntensity1) * pow(max(dot(R,E),0.0), shininess); //light colour multiplied by exposure multiplied by specular terms. 
		Ispec = clamp(Ispec, 0.0, 1.0);
	}

	posLoc = vec4(v, 1.0f);
	fragmentColor = vec4((Iamb + Idiff + Ispec).xyz, 1.0f); //Iamb; //replace this with Iamb when using deffered rendering.
	normalColor = vec4(N, 1.0f);
}