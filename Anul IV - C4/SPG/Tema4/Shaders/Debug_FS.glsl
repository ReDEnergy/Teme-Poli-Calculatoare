#version 410
layout(location = 0) in vec2 texture_coord;

// Debug Textures
uniform sampler2D u_texture_0;	
uniform sampler2D u_texture_1;
uniform sampler2D u_texture_2;
uniform sampler2D u_texture_3;
uniform sampler2D u_texture_4;
uniform sampler2D u_texture_5;
uniform sampler2D u_texture_6;
uniform sampler2D u_texture_7;

layout(location = 0) out vec4 frag_color;

void main() {
	vec4 frag_color = texture(u_texture_0, texture_coord);
 }