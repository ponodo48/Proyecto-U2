#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

layout(location = 1) in vec2 vertexUV;

layout(location = 2) in vec3 vertexNormal_modelspace;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;

uniform mat4 rot;
uniform vec3 scale;
uniform vec3 pos;
uniform vec3 LightPosition_worldspace;

uniform mat4 rotGlobal;
uniform vec3 scaleGlobal;
uniform vec3 posGlobal;


vec4 modelPosition;
vec4 normalPosition;

vec3 LightPosition_worldspace2 = vec3(0,2,2);

out vec2 UV;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightDirection_cameraspace2;


void main(){
	vec4 modelPosition = (rot*vec4(scale*vertexPosition_modelspace,1)+vec4(pos,0));
	vec3 modelPos3 = vec3(modelPosition.x, modelPosition.y, modelPosition.z);
	modelPosition = (rotGlobal*vec4(scaleGlobal*modelPos3,1)+vec4(posGlobal,0));
	
	if(modelPosition.y <0){
		modelPosition.y = 0;
	}
	//modelPosition += vec4(sin(modelPosition.y*20)*0.3,0,0,0);
    vec4 normalPosition = rot*vec4(vertexNormal_modelspace,1);
    //gl_Position = MVP*modelPosition;

    // Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * modelPosition;
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * modelPosition).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * modelPosition).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;	

	vec3 LightPosition_cameraspace2 = ( V * vec4(LightPosition_worldspace2,1)).xyz;
	LightDirection_cameraspace2 = LightPosition_cameraspace2 + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * normalPosition).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}
