#version 410

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texture_coord;
layout(location = 2) in vec3 v_normal;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) out vec2 texture_coord;
layout(location = 1) out vec3 world_position;
layout(location = 2) out vec3 world_normal;
layout(location = 3) out vec3 view_position;
layout(location = 4) out vec3 view_normal;
layout(location = 5) out float zDepth;

void main() {
	texture_coord = v_texture_coord;

	world_position = (Model * vec4(v_position, 1.0)).xyz;
	world_normal = mat3(Model) * v_normal;

	view_position = (View * Model * vec4(v_position, 1.0)).xyz;
	view_normal = mat3(View) * mat3(Model) * v_normal;
	
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);

	// Linearize the zBuffer
	// zDepth = (gl_Position.z * gl_Position.w / 5000.0) * 2.0 - 1.0;
	// gl_Position.z = zDepth;
	
}
