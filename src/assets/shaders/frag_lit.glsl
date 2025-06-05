

#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

struct Material {
    sampler2D diffuse;
    // vec3 diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;

uniform Material material;

uniform Light light;  
uniform vec3 viewPos;

void main()
{   
    vec3 diffuseValue = vec3(texture(material.diffuse, TexCoords));
    vec3 specularValue = vec3(texture(material.specular, TexCoords));
    
    // ambient
    vec3 ambient = light.ambient * diffuseValue;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * diffuseValue);  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * specularValue);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
    // float c = sin(FragPos.y*2*3.14*2+0.5)/2+0.5;
    //FragColor = vec4(c, c, c, 1.0);

}

