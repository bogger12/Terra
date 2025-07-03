

#version 330 core

// LIGHTS

// struct DirLight {
//     vec3 direction;
  
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };  

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
	vec3 viewPos;
};

layout (std140) uniform DirLight
{
    vec3 dirdirection;
    vec3 dirambient;
    vec3 dirdiffuse;
    vec3 dirspecular;
};


// struct PointLight {
//     vec3 position;
    
//     float constant;
//     float linear;
//     float quadratic;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };  
// struct SpotLight {
//     vec3 position;

//     vec3 direction;
//     float cutOff;
//     float outerCutOff;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };

// uniform DirLight dirLight;

// #define MAX_POINT_LIGHTS 2
// uniform PointLight pointLights[MAX_POINT_LIGHTS];
// #define MAX_SPOT_LIGHTS 4
// uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

// uniform int numPointLights;
// uniform int numSpotLights;

// MATERIAL

struct ColorMaterial {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};
uniform ColorMaterial colorMat;
bool useColor = colorMat.diffuse != vec3(0.0);


uniform sampler2D texture_diffuse1;
// vec3 diffusecolor;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

// uniform samplerCube reflectionMap;
uniform float shininess;

// uniform vec3 viewPos;


out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;
in mat3 TBN;
in float FresnelEffect;
in vec3 ViewDir;


vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir);
// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 getAmbient() {
    if (!useColor) return vec3(texture(texture_diffuse1, TexCoords));
    else return colorMat.ambient;
}
vec3 getDiffuse() {
    if (!useColor) return vec3(texture(texture_diffuse1, TexCoords));
    else return colorMat.diffuse;
}
vec3 getSpecular() {
    if (!useColor) return vec3(texture(texture_specular1, TexCoords));
    else return colorMat.specular;
}

float near = 0.1; 
float far  = 10.0; 
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{   
    // Properties
    vec3 normal;
    //     FragColor = vec4(dirdirection, 1.0);
    // return;

    
    if (useColor) normal = normalize(Normal);
    else {  
        normal = texture(texture_normal1, TexCoords).rgb; // use normal map
        if (length(normal)<=1.0) normal = normalize(Normal); 
        else {
            // transform normal vector to range [-1,1]
            normal = normal * 2.0 - 1.0;  // this normal is in tangent space
            normal = normalize(TBN * normal); 
        }

    }

    float viewDistance = length(viewPos - FragPos);

    // float depth = 1 - (LinearizeDepth(gl_FragCoord.z) / 2);
    // float depth = clamp(2 - (viewDistance / 3),0,1);
    // FragColor = vec4(vec3(depth), 1.0);
    // return;

    // define an output color value
    vec3 result = vec3(0.0);

    result += FresnelEffect * vec3(0.2);
    
    // Reflection test
    // if (gl_FragCoord.x > 1280) {
    //     float ratio = 1.00 / 1.52;
    //     vec3 R = reflect(-viewDir, normal);
    //     // // vec3 R = refract(-viewDir, normal, ratio);
    //     // FragColor = vec4(texture(reflectionMap, R).rgb, 1.0);
    //     return;
    // } else {
    //     result += CalcDirectionalLight(normal, viewDir);
    //     FragColor = vec4(result, 1.0);
    //     return;
    // }
    // FragColor = vec4(vec3(gl_FragCoord.x > 800), 1.0);

    // phase 1: Directional lighting
    result += CalcDirectionalLight(normal, ViewDir);
    // phase 2: Point lights
    // for(int i = 0; i < numPointLights; i++)
    //     result += CalcPointLight(pointLights[i], normal, FragPos, viewDir);
    // // phase 3: Spot lights
    // for(int i = 0; i < numSpotLights; i++)
    //     result += CalcSpotLight(spotLights[i], normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}



vec3 CalcDirectionalLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirdirection);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = dirambient  * getAmbient();
    vec3 diffuse  = dirdiffuse  * diff * getDiffuse();
    vec3 specular = dirspecular * spec;
    return (ambient + diffuse + specular);
}  

// vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
// {
//     vec3 lightDir = normalize(light.position - fragPos);
//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//     // attenuation
//     float distance    = length(light.position - fragPos);
//     float attenuation = 1.0 / (light.constant + light.linear * distance + 
//   			     light.quadratic * (distance * distance));    
//     // combine results
//     vec3 ambient  = light.ambient  * getAmbient();
//     vec3 diffuse  = light.diffuse  * diff * getDiffuse();
//     vec3 specular = light.specular * spec * getSpecular();
//     ambient  *= attenuation;
//     diffuse  *= attenuation;
//     specular *= attenuation;
//     return (ambient + diffuse + specular);
// } 


// vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
//     vec3 lightDir = normalize(light.position - fragPos);

//     float theta = dot(lightDir, normalize(-light.direction));
//     float epsilon = light.cutOff - light.outerCutOff;
//     float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

//     // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//     // specular shading
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
//     // combine results
//     vec3 ambient  = light.ambient  * getAmbient();
//     vec3 diffuse  = light.diffuse  * diff * getDiffuse();
//     vec3 specular = light.specular * spec * getSpecular();
//     ambient  *= intensity;
//     diffuse  *= intensity;
//     specular *= intensity;
//     return (ambient + diffuse + specular);
// }