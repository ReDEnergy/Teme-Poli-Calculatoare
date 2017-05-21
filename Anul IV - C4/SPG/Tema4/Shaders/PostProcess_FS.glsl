#version 410
layout(location = 0) in vec2 texture_coord;

#define Diffuse	u_texture_0
#define World	u_texture_1
#define Normals	u_texture_2
#define Texture	u_texture_3
#define DepthT	u_texture_7

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// World position
uniform sampler2D u_texture_2;	// Normals texture
uniform sampler2D u_texture_3;	// Texture coords
uniform sampler2D u_texture_7;	// Depth buffer

uniform vec3 light_position;
uniform vec3 eye_position;
uniform vec2 resolution;

layout(location = 0) out vec4 frag_color;

void main() {
	vec2 text_coord = gl_FragCoord.xy / resolution;
	vec4 diffuse = texture(DepthT, text_coord);
	vec4 orginal = texture(World, text_coord);
	if (orginal.r != 0.0)
		frag_color = orginal;
	if (diffuse.r != 0.0)
		frag_color = diffuse;
	if (diffuse.b != 0.0)
		frag_color = diffuse;
	if (diffuse.g != 0.0)
		frag_color = diffuse;
	if (orginal.r == 0)
		frag_color = orginal;
	// frag_color = diffuse;
		
		
	// vec3 world_pos = (texture(World, text_coord)).rgb;
	// vec3 normal = normalize((texture (Normals, text_coord)).rgb);
  	// vec4 light_color = PhongLight(world_pos, normal);
}
