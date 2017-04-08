//Rename test shader to phong shader, and implement phong shading with normal maps

#version 430
layout(triangles) in;

// Three lines will be generated: 6 vertices * 3 coordinates = 18
layout(line_strip, max_vertices=18) out; //was 6
 
in Vertex1
{
	vec4 vertPos;
	vec4 normal;
	vec4 color;
	vec4 tangent;
	vec4 binormal;
} vertex[];

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

out vec4 vertexColor;

void main() 
{	
	mat4 MVP = MVPMatrix;
	float normal_length = 0.15f;
	int i;
	
	for(i=0; i <= gl_in.length(); i++)
	{
		vec3 P = vertex[i].vertPos.xyz; //gl_in[i].gl_Position.xyz;
		vec3 N = vertex[i].normal.xyz;
		vec3 T = vertex[i].tangent.xyz;
		vec3 BN = vertex[i].binormal.xyz;
    
		//Vertex Normal Vector
		gl_Position = MVP * vec4(P, 1.0);
		vertexColor = vec4(0.2, 0.2, 0.7, 1.0);
		EmitVertex();
    
		gl_Position =  MVP * vec4(P + N * normal_length, 1.0);
		vertexColor = vec4(0.2, 0.2, 0.7, 1.0);
		EmitVertex();
		
		//Vertex Tangent Vector
		gl_Position = MVP * vec4(P, 1.0); 
		vertexColor = vec4(0.7, 0.2, 0.1, 1.0);
		EmitVertex();
    
		gl_Position =  MVP * vec4(P + T * normal_length, 1.0);
		vertexColor = vec4(0.7, 0.2, 0.1, 1.0);
		EmitVertex();
		
		//Vertex Binormal Vector
		gl_Position = MVP * vec4(P, 1.0);
		vertexColor = vec4(0.055, 1.0, 0.0078, 1.0);
		EmitVertex();
    
		gl_Position =  MVP * vec4(P + BN * normal_length, 1.0);
		vertexColor = vec4(0.055, 1.0, 0.0078, 1.0);
		EmitVertex();
    
		EndPrimitive();
	}
}