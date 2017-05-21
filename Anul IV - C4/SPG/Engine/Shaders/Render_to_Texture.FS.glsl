#version 410
#define Diffuse	u_texture_0
#define Alpha	u_texture_1

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
layout(location = 3) in vec3 view_position;
layout(location = 4) in vec3 view_normal;
layout(location = 5) in float zDepth;

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// Alpha texture

uniform mat4 View;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec2 resolution;
uniform bool use_alpha_mask;

layout(location = 0) out vec3 out_color;
layout(location = 1) out vec3 out_world_position;
layout(location = 2) out vec3 out_world_normal;
layout(location = 3) out vec3 out_view_position;
layout(location = 4) out vec3 out_view_normal;

vec4 PhongLight(vec3 w_pos, vec3 w_N);
vec4 CalcSpotLight(vec3 w_pos, vec3 w_N);

void main() {
	if (use_alpha_mask) {
		vec3 alpha = texture(Alpha, texture_coord).rgb;
		if(alpha.r < 0.1 && alpha.g < 0.1 && alpha.b < 0.1)
			discard;
	}
	
	out_color = texture(Diffuse, texture_coord).rgb;
	out_world_position = world_position;
	out_world_normal = normalize(world_normal);
	out_view_position = view_position;
	out_view_normal = normalize(view_normal);
	
	// vec3 light_color = PhongLight(world_position, out_world_normal).xyz;
	

	// Gamma Correction
	// frag_color = pow(frag_color, vec3(1.0 / 2.2));
}

// -----------------------------------------------------------------------------
// Phong Light

const vec3 ld = vec3 (1.0, 1.0, 1.0);	// Diffuse factor
const vec3 ls = vec3 (1.0, 1.0, 1.0);	// Specular factor
const float specular_exponent = 40.0;	// Specular exponent
const vec3 light_dir = vec3 (0.0, -1.0, 0.0);
const float cut_off = 0.75;
 
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

vec4 CalcSpotLight(vec3 w_pos, vec3 w_N)
{
	vec3 light_direction  = normalize(w_pos - light_position);
	float spot_factor = dot(light_direction, light_dir);

	if (spot_factor > cut_off) {
		vec4 color = PhongLight(w_pos, w_N);
		return color * (1.0 - (1.0 - spot_factor) / (1.0 - cut_off));
	}
	return vec4(0, 0, 0, 0);
}
