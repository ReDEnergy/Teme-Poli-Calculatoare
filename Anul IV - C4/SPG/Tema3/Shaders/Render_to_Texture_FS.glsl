#version 410
#define Diffuse	u_texture_0
#define Alpha	u_texture_1
#define DepthT	u_texture_7

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;
layout(location = 3) in vec4 shadow_coord;

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// Alpha texture
uniform sampler2D u_texture_7;	// Depth buffer

uniform vec3 light_position;
uniform vec3 eye_position;

layout(location = 0) out vec3 frag_color;
layout(location = 1) out vec3 frag_position;
layout(location = 2) out vec3 frag_normal;
layout(location = 3) out vec2 frag_texture_coord;

float EvalShadow();
vec4 PhongLight(vec3 w_pos, vec3 w_N);
vec4 CalcSpotLight(vec3 w_pos, vec3 w_N);

void main() {
	vec3 frag_diffuse = texture(Diffuse, texture_coord).xyz;
	vec3 alpha = texture(Alpha, texture_coord).xyz;
	if(alpha.r < 0.1 && alpha.g < 0.1 && alpha.b < 0.1)
		discard;
	
	frag_position = world_position;
	frag_normal = world_normal;
	frag_texture_coord = texture_coord;

	float shadow_factor = EvalShadow();

	if (shadow_factor != 1.0)
		frag_color = frag_diffuse * shadow_factor;
	else {
	  	vec3 light_color = PhongLight(frag_position, frag_normal).xyz;
		frag_color = frag_diffuse * 0.75 + light_color;
	}
}

// -----------------------------------------------------------------------------
// Percentage Closer Filtering (PCF) -------------------------------------------

vec2 texel_offset = vec2(1.0/4096, 1.0/4096);
bool inLight(vec2 offset) {
	float depth = texture(DepthT, shadow_coord.xy + offset * texel_offset).z;
	if (depth + 0.003 < shadow_coord.z)
		return false;
	return true;
}

float PCFfactor() {
	float x, y;
	float sum = 0;
	for (y = -1.5; y <= 1.5; y += 1.0)
	  for (x = -1.5; x <= 1.5; x += 1.0)
		if (inLight(vec2(x, y)))
			sum += 1.0;
	return sum / 16;
}

float EvalShadow() {
	float visibility = 1.0; // not shadowed
	if (!inLight(vec2(0.0, 0.0))) {
		// visibility = 0.2;
		visibility = mix(0.2, 1.0, PCFfactor());
	}
	return visibility;
}

// -----------------------------------------------------------------------------
// Phong Light -----------------------------------------------------------------

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
