#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = 1.0f;
	vec3 ambient = ambientStrength * lightColor;
	
	vec4 lighting = vec4(ambient, 1.0f);
	
    FragColor = texture(texture_diffuse1, TexCoords) * lighting;
}