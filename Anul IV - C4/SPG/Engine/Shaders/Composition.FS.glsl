#version 410

#define Diffuse	u_texture_0

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// World positions
uniform sampler2D u_texture_2;	// World normals
uniform sampler2D u_texture_3;	// View positions
uniform sampler2D u_texture_4;	// View normals
uniform sampler2D u_texture_5;	// Ambient occlusion
uniform sampler2D u_texture_6;	// Deferred light
uniform sampler2D u_texture_7;	// Depth buffer

uniform vec2 resolution;

layout(location = 0) out vec4 out_color;

void main() {
	vec2 text_coord = gl_FragCoord.xy / resolution;

	vec4 diffuse = texture(u_texture_0, text_coord);
	vec4 ssao = texture(u_texture_5, text_coord);
	vec4 light = texture(u_texture_6, text_coord);
	vec4 ambient = vec4(0.3);
	
	// if (text_coord.x > 0.5 || text_coord.y < 0.5)
	out_color = clamp((diffuse - (vec4(1.0) - ssao)), 0.0, 1.0);
	// out_color = diffuse;
	
	// Test if AO and blurred AO
	// out_color = out_color = text_coord.x < 0.5 ? ssao : ao;
	out_color *= (light + ambient);
}