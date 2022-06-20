#version 450 core

out vec4 FragColor;

//Informações provenientes do vertex shader
in vec2 TexCoords;
in vec3 scaledNormal; //vetor normal com correção de escala
in vec3 fragPos; //posição em coord de mundo do objeto


//Informações provenientes do código em C++
uniform sampler2D texture_diffuse1;		//textura
uniform vec3 lightPos;      //posiçao da fonte de luz
uniform vec3 viewPos;       //posicao da camera
uniform vec3 lightColor;    //cor da fonte de luz

void main()
{
	// Ambient
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightColor * 0.5;
	vec4 ambientLighting = vec4(ambient, 1.0f);
	
	// Diffuse
	vec3 norm = normalize(scaledNormal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
	vec4 diffuseLighting = vec4(diffuse, 1.0f);
	
	// Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5);
	vec3 specular = spec * lightColor;
	vec4 specularLighting = vec4(specular, 1.0f);
	
	
	vec4 lightingResult = ambientLighting + diffuseLighting;
	
	
    FragColor = (lightingResult * texture(texture_diffuse1, TexCoords) + specularLighting) * vec4(0.16, 0.66, 0.9, 1.0);
}
