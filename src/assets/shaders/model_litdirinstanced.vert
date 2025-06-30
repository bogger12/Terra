

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in mat4 matrix;

out vec2 TexCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	vec3 viewPos;
};

void main()
{
	// note that we read the multiplication from right to left
    gl_Position = projection * view * matrix * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
}

