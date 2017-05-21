#version 410

#define Alpha	u_texture_1

layout(location = 0) in vec2 texture_coord;

uniform sampler2D u_texture_1;	// Alpha texture

void main() {

	vec3 alpha = texture(Alpha, texture_coord).xyz;
	if(alpha.r < 0.1 && alpha.g < 0.1 && alpha.b < 0.1)
		discard;

}