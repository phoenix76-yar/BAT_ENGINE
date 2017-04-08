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
};

uniform sampler2D albedoTexture;
uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;

in VertexInfo
{
	vec4 vertPos1;
	vec2 vertUVCh1;
};

//out vec4 fragmentColor;
layout(location = 0) out vec4 fragmentColor;

void main(void)  
{ 
	vec3 lpos = vec3(-2,15,4);

	vec3 lightPositions[8];
	lightPositions[0] = vec3(-2,15,5);
	lightPositions[1] = vec3(-2,0,5);
	lightPositions[2] = vec3(-2,-15,5);
	lightPositions[3] = vec3(-2,-30,5);

	
	lightPositions[4] = vec3(15,15,5);
	lightPositions[5] = vec3(15,0,5);
	lightPositions[6] = vec3(15,-15,5);
	lightPositions[7] = vec3(15,-30,5);
	


	vec4 albedo = texture(albedoTexture, vec2(vertUVCh1.x,( 1 - vertUVCh1.y))); //  * vec4(0.2, 0.4, 0.3, 1.0);
	vec4 position = texture(positionTexture, vec2(vertUVCh1.x,( 1 - vertUVCh1.y)));
	vec4 normal = texture(normalTexture, vec2(vertUVCh1.x,( 1 - vertUVCh1.y)));
	vec4 depth = texture(depthTexture, vec2(vertUVCh1.x,( 1 - vertUVCh1.y)));
	
	
	vec3 N = normal.xyz;
	vec3 v = position.xyz;	
	
	vec4 Iamb;
	vec4 Idiff;
	vec4 Ispec;
	
	//calculate Ambient Term:
	Iamb = albedo * vec4(0.2, 0.4, 0.3, 1.0); //This could be the place where to add AO map.
	
	for (int i = 0; i < 4; i++)
	{
		lpos = lightPositions[i];
	
		//light vector/light direction
		vec3 L = normalize(((ViewMatrix * vec4(lpos,1)).xyz - v));
		
		//Transform light dir. and view dir. to tangent space.
		L = normalize(L);
		//v = normalize(v); //Here only v was suppsed to be normalized
		
		vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
		vec3 R = normalize(-reflect(L, N));
		
		//calculate Diffuse Term:
		vec4 Idiff2 = vec4(0);
		Idiff2 = albedo * max(dot(N,L), 0.0) * vec4(0.9, 0.4 * (i + 1), 0.2 * (i + 1), 1.0);
		Idiff += clamp(Idiff2, 0.0, 1.0); //This is what causes extra brightness
		
		//calculate Specular Term:
		float shininess = 100;
		
		float dist = length(lpos - v); //was L - v
		float attenuation = 1.0 / (dist * dist);
		float lightIntensity1 = attenuation * 100;//0.1f;
		
		vec4 Ispec2 = vec4(0);
		if(max(dot(N, L), 0.0f) > 0) //be carefull with this, was without max(x,0.0f)
		{
			Ispec2 = (vec4(0.9, 0.4 * (i + 1), 0.2 * (i + 1), 1.0) * lightIntensity1) * pow(max(dot(R,E),0.0), shininess); //light colour multiplied by exposure multiplied by specular terms. 
			Ispec += clamp(Ispec2, 0.0, 1.0);
		}
	}
	
	vec3 color = (Iamb + Idiff + Ispec).xyz;
	//color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2)); 
	
	fragmentColor = vec4(color, 1.0f);
}