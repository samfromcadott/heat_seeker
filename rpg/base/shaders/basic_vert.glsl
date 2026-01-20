#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;
uniform mat4 matNormal;
uniform vec3 camera_pos;

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

const vec3 light_dir = vec3(1.0, 1.0, 1.0);
const vec4 light_color = vec4(1.0, 1.0, 1.0, 1.0);
const float shininess = 2.0;

void main() {
	vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

	 // Transform coords
	fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));
	fragTexCoord = vertexTexCoord;
	fragNormal = normalize( vec3(matNormal*vec4(vertexNormal, 1.0)) );

	// Vector to light
	vec3 light = normalize(light_dir);
	float NdotL = max(dot(fragNormal, light), 0.0);
	vec4 diffuse = light_color * NdotL;
	diffuse.a = 1.0;

	// Compute specular
	float specularStrength = 0.0;
	// float specularStrength = 0.5;
	vec3 viewDir = normalize(camera_pos - vertexPosition);
	vec3 reflectDir = reflect(-light, fragNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec4 specular = specularStrength * spec * light_color;

	// fragColor = diffuse + specular + global + ambient;
	fragColor = vertexColor * diffuse +  vertexColor * ambient + specular;
// 	fragColor = vertexColor * diffuse +  vertexColor * ambient;
// 	fragColor = vec4(1.0, 1.0, 0.0, 1.0);

	// Calculate final vertex position
	gl_Position = mvp*vec4(vertexPosition, 1.0);
}
