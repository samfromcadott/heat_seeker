R"(
#version 330

struct Light {
	vec3 position;
	vec4 color;
	float power;
};

#define MAX_LIGHTS 16

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
uniform Light lights[MAX_LIGHTS];

// Output vertex attributes (to fragment shader)
out vec3 fragPosition;
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;

const vec3 light_dir = vec3(1.0, 1.0, 1.0);
const vec4 light_color = vec4(1.0, 1.0, 1.0, 1.0);
const float shininess = 2.0;

vec4 calc_light(Light light, vec3 pos, vec3 norm) {
	vec3 dir = normalize(light.position - pos);
	float dist = distance(pos, light.position);
	float attenuation = 1.0 / (dist*dist*8.0);

	// Diffuse
	float NdotL = max(dot(norm, dir), 0.0);
	vec4 diffuse = light.color * NdotL * light.power * attenuation;
	diffuse.a = 1.0;
	diffuse *= vertexColor;

	return diffuse;
}

void main() {
	vec4 ambient = vec4(0.3, 0.3, 0.3, 1.0);

	 // Transform coords
	fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));
	fragTexCoord = vertexTexCoord;
	fragNormal = normalize( vec3(matNormal*vec4(vertexNormal, 1.0)) );

	// Calculate lighting
	fragColor = vertexColor * ambient;

	for (int i = 0; i < MAX_LIGHTS; i++) {
		fragColor += calc_light(lights[i], fragPosition, fragNormal);
	}

	// Calculate final vertex position
	gl_Position = mvp*vec4(vertexPosition, 1.0);
}
)"
