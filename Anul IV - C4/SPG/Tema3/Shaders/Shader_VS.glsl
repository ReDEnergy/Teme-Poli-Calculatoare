#version 410

layout(location = 0) in vec3 in_position;		
layout(location = 1) in vec2 in_texture_coord;
layout(location = 2) in vec3 in_normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;

void main() {
	texture_coord = in_texture_coord;
	world_position = (Model * vec4(in_position, 1.0)).xyz;
	world_normal = normalize( mat3(Model) * in_normal );
	gl_Position = Projection * View * Model * vec4(in_position, 1.0);
}
