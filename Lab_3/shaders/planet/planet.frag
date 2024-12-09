#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D bumpMap;
uniform sampler2D specularMap;
uniform sampler2D cloudMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ambientLightColor;

uniform float lightIntensity;
uniform float time; 

void main() {
    vec3 diffuseColor = texture(diffuseMap, TexCoords).rgb;

    vec3 normal = vec3(0.0, 1.0, 0.0); // Базовый нормаль
    if (textureSize(normalMap, 0).x > 0) {
        normal = texture(normalMap, TexCoords).rgb * 2.0 - 1.0;
    }

    if (textureSize(bumpMap, 0).x > 0) {
        float height = texture(bumpMap, TexCoords).r * 0.05;
        normal += vec3(height, height, height);
    }
    normal = normalize(normal);

    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseColor * lightIntensity;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = spec * lightColor * lightIntensity * 0.5;

    vec3 ambient = ambientLightColor * diffuseColor;

    vec3 cloudColor = vec3(0.0);
    if (textureSize(cloudMap, 0).x > 0) {
        float rotationSpeed = 0.01;
        vec2 rotatedCoords = TexCoords + vec2(time * rotationSpeed, 0);
        cloudColor = texture(cloudMap, rotatedCoords).rgb;

        cloudColor = mix(diffuseColor, cloudColor, 0.5);
    }

    vec3 finalColor = ambient + diffuse + specular;
    finalColor = mix(finalColor, cloudColor, 0.5);
    FragColor = vec4(finalColor, 1.0);
}
