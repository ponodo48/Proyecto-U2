#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightDirection_cameraspace2;

// Ouput data
out vec3 color;
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace;


vec3 LightPosition_worldspace2 = vec3(0,2,2);

void main(){
	
	// Light emission properties
	// You probably want to put them as uniforms
	vec3 LightColor = vec3(0,0,1);

	vec3 LightColor2 = vec3(1,0,0);
	float LightPower = 50.0f;

	// Material properties
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.7,0.7,0.7);

	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );
	float distance2 = length( LightPosition_worldspace2 - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );

	vec3 l2 = normalize( LightDirection_cameraspace2 );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	float cosTheta2 = clamp( dot( n,l2 ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	vec3 R2 = reflect(-l2,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	float cosAlpha2 = clamp( dot( E,R2 ), 0,1 );
	
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance) +
		// Diffuse2 : "color" of the object
		MaterialDiffuseColor * LightColor2 * LightPower * cosTheta2 / (distance2*distance2) +
		// Specular2 : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor2 * LightPower * pow(cosAlpha2,5) / (distance2*distance2);
		;

}