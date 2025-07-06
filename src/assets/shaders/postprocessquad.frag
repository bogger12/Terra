#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D renderTexture;
uniform sampler2D depthTexture;

float near = 0.1; 
float far  = 10.0; 
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

const float offset = 1.0 / 800.0;

void main()
{
    float depth = 1-LinearizeDepth(texture(depthTexture, TexCoords).r)/far;
    // FragColor = vec4(vec3(depth), 1.0);
    // return;
    FragColor = vec4(vec3(texture(renderTexture, TexCoords)), 1.0);
    return;
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1, 1, 1,
        1,  -8, 1,
        1, 1, 1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        vec2 texcoord = (TexCoords.st + offsets[i]);
        if (texcoord.x < 0 || texcoord.y < 0 || texcoord.x > 1.0 || texcoord.y > 1.0) {
            sampleTex[i] = vec3(0.0);
            continue;
        }
        sampleTex[i] = vec3(texture(renderTexture, texcoord));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
} 