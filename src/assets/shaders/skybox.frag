#version 330 core

#define NUM_LIGHTS 4 

layout (std140) uniform Lights
{
    vec3 direction;
};

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{   
    float minLightness = 0.3;
    float lightness = clamp(dot(vec3(0,-1,0), normalize(direction)), minLightness, 1);
    FragColor = texture(skybox, TexCoords) * lightness;
}