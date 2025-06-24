

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;


// LIGHTS

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
struct SpotLight {
    vec3 position;

    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_POINT_LIGHTS 2
#define MAX_SPOT_LIGHTS 4

out LIGHTS {
	DirLight dirLight;

	PointLight pointLights[MAX_POINT_LIGHTS];
	SpotLight spotLights[MAX_SPOT_LIGHTS];

	int numPointLights;
	int numSpotLights;
} tangentLights;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int numPointLights;
uniform int numSpotLights;

uniform vec3 viewPos;


// out vec3 Normal;
out vec3 FragPos;
out vec3 TangentFragPos;
out vec3 TangentViewPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	// note that we read the multiplication from right to left
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;

	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);

	TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;

	// Lights
	
	for(int i = 0; i < numPointLights; i++) {
		tangentLights.pointLights[i] = pointLights[i];
        tangentLights.pointLights[i].position = TBN * pointLights[i].position;
	}
    // phase 3: Spot lights
    for(int i = 0; i < numSpotLights; i++) {
		tangentLights.spotLights[i] = spotLights[i];
        tangentLights.spotLights[i].direction = TBN * spotLights[i].direction;
	}

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}

