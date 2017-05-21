#version 410

#define Diffuse	u_texture_0
#define World	u_texture_1
#define Normals	u_texture_2
#define Texture	u_texture_3
#define Noise1	u_texture_4
#define Noise2	u_texture_5
#define DepthT	u_texture_7

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// World position
uniform sampler2D u_texture_2;	// Normals texture
uniform sampler2D u_texture_3;	// Texture coords
uniform sampler2D u_texture_4;	// Random Texture 1
uniform sampler2D u_texture_5;	// Random Texture 2
uniform sampler2D u_texture_7;	// Depth buffer

uniform vec2 resolution;

layout(location = 0) out vec4 out_color;

void main() {
	vec2 text_coord = gl_FragCoord.xy / resolution;
	out_color = texture(Diffuse, text_coord);
}
