#version 330 core

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	vec3 viewPos;
};

uniform sampler2D texture_diffuse1;

out vec4 FragColor; 
in vec2 TexCoords;

void main()
{   
    // Properties
    vec3 normal;

    FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0);
    return;
}