#version 410
layout(location = 0) in vec2 texture_coord;

uniform sampler2D u_texture_0;	// Debug Texture
uniform sampler2D u_texture_1;	// Debug Texture
uniform sampler2D u_texture_2;	// Debug Texture
uniform sampler2D u_texture_3;	// Debug Texture
uniform sampler2D u_texture_4;	// Debug Texture

layout(location = 0) out vec4 frag_color;

vec4 depth() {
	float d = pow(texture(u_texture_0, texture_coord).x , 256);
	return vec4(d, d, d, 1.0);
}

void main() {
	// vec4 color = texture(u_texture_0, texture_coord);
	frag_color = depth();
 }