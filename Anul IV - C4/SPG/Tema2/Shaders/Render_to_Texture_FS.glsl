#version 410

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;

uniform sampler2D u_texture_0;
uniform vec3 light_position;
uniform vec3 eye_position;

layout(location = 0) out vec3 frag_color;
layout(location = 1) out vec3 frag_position;
layout(location = 2) out vec3 frag_normal;
layout(location = 3) out vec2 frag_texture_coord;

vec4 PhongLight(vec3 w_pos, vec3 w_N);

void main() {

	vec3 frag_diffuse = texture(u_texture_0, texture_coord).xyz;
	frag_position = world_position;
	frag_normal = world_normal;
	frag_texture_coord = texture_coord;

  	vec3 light_color = PhongLight(frag_position, frag_normal).xyz;
	frag_color = frag_diffuse * 0.75 + light_color;
}

// Phong Light constants
const vec3 ld = vec3 (1.0, 1.0, 1.0);	// Diffuse factor
const vec3 ls = vec3 (1.0, 1.0, 1.0);	// Specular factor
const float specular_exponent = 40.0;	// Specular exponent
 
vec4 PhongLight(vec3 w_pos, vec3 w_N)
{
	vec3 L = normalize(light_position - w_pos);
	vec3 V = normalize(eye_position - w_pos);
	vec3 H = normalize(L + V);

	float d = distance(light_position, w_pos) / 30;
	float att = 1.0f / pow(d, 2);
	att = 0.3;
	
	float dot_specular = dot(w_N, L);
	vec3 specular = vec3(0);
	if (dot_specular > 0)
		specular = ls * pow(max(dot(w_N, H), 0), specular_exponent);

	vec3 diffuse = ld * max(dot_specular, 0);

	return vec4(att * (diffuse + specular), 1.0);
}
