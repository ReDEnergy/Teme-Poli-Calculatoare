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

vec4 depth(sampler2D depthTexture, vec2 coord) {
	float d = pow(texture(depthTexture, coord).x , 256);
	return vec4(d, d, d, 1.0);
}

void main() {
	frag_color = texture(u_texture_6, texture_coord);
	// frag_color = texture(u_texture_6, texture_coord * vec2(210, 120));
}