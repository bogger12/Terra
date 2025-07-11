#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	vec3 viewPos;
};
uniform mat4 model;

out vec3 color;
uniform vec3 endColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    color = mix(vec3(1.0,1.0,1.0), endColor, aPos.z);
}