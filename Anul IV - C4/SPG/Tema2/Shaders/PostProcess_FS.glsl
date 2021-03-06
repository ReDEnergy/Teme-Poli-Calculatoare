#version 410

layout(location = 0) in vec2 texture_coord;

#define Diffuse	u_texture_0
#define World	u_texture_1
#define Normals	u_texture_2
#define Texture	u_texture_3

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// World position
uniform sampler2D u_texture_2;	// Normals texture
uniform sampler2D u_texture_3;	// Texture coords

uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec2 window_resolution;

layout(location = 0) out vec4 frag_color;

vec4 PhongLight(vec3 w_pos, vec3 w_N);
vec4 CalcSpotLight(vec3 w_pos, vec3 w_N);

void main() {
	vec2 text_coord = gl_FragCoord.xy / window_resolution;

	vec4 diffuse = texture(Diffuse, texture_coord);
	// vec3 world_pos = (texture(World, text_coord)).rgb;
	// vec3 normal = normalize((texture (Normals, text_coord)).rgb);
  	// vec4 light_color = PhongLight(world_pos, normal);

	frag_color = diffuse; // + 0.75 * light_color;
}

const vec3 kd = vec3 (0.8, 0.8, 0.8);
const vec3 ks = vec3 (0.8, 0.8, 0.8);
const vec3 ld = vec3 (1.0, 1.0, 1.0);
const vec3 ls = vec3 (1.0, 1.0, 1.0);
const vec3 la = vec3 (0.2, 0.2, 0.2);
const float specular_exponent = 40.0;
const vec3 light_dir = vec3 (0.0, -1.0, 0.0);
const float cut_off = 0.75;

vec4 PhongLight(vec3 w_pos, vec3 w_N)
{
	vec3 L = normalize(light_position - w_pos);
	vec3 V = normalize(eye_position - w_pos);
	vec3 H = normalize(L + V);

	float d = distance(light_position, w_pos) / 50;
	float att = 1.0f / pow(d, 2);
	
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