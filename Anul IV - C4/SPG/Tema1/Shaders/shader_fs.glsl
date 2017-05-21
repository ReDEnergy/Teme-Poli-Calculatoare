#version 410

layout(location = 0) in vec2 texture_coord;
layout(location = 1) in vec3 world_position;
layout(location = 2) in vec3 world_normal;

uniform mat4 Model;
uniform sampler2D u_texture;

uniform vec3 light_position;
uniform vec3 eye_position;

layout(std140) uniform Material
{
	vec4    mat_ambient;
	vec4    mat_diffuse;
	vec4    mat_specular;
	float   mat_shininess;
};

layout(location = 0) out vec4 frag_color;

void main() {
	vec4 texel = texture(u_texture, texture_coord);
	vec3 light = vec3(1.0, 1.0, 1.0);

	vec3 L = normalize ( light_position - world_position);
	vec3 V = normalize ( eye_position - world_position);
	vec3 N = world_normal;
	vec3 H = normalize(L + V);

	float d = distance( light_position, world_position) / 20;
	float att = 1.0f / pow(d, 2);
	float dot_specular = dot(N, L);
	float has_light = 0;
	if (dot_specular > 0)
		has_light = 1;

	vec3 ambient = mat_ambient.rgb * light;
	vec3 diffuse = mat_diffuse.rgb * light * max(dot_specular, 0);
	vec3 specular = mat_specular.rgb * light * has_light * pow(max(dot(N, H), 0), mat_shininess );

	vec4 light_color = vec4(ambient + att * (diffuse + specular), 1.0);

	frag_color = texel * light_color;
 }