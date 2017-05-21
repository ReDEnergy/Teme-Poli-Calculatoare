#version 410

layout(location = 0) in vec3 v_position;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

layout(location = 0) out vec3 light_position;

void main() {
	light_position = (Model * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
