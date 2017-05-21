// -----------------------------------------------------------------------------
// Declare matrix GLSL

mat4 matrix = mat4(
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 0.5, 0.0,
	0.5, 0.5, 0.5, 1.0
);

// -----------------------------------------------------------------------------
// General Texture Sampling Function

vec2 texel_offset = vec2(1.0/resolution);
vec4 getSample(sampler2D u_texture, vec2 text_coords, vec2 offset) {
	vec4 color = texture(u_texture, text_coords + offset * texel_offset);
	return color;
}

// -----------------------------------------------------------------------------
// Shadow Mapping - Percentage Closer Filtering (PCF)

vec2 texel_offset = vec2(1.0/4096, 1.0/4096);
bool inLight(vec2 offset) {
	float depth = texture(depth_texture, shadow_coord.xy + offset * texel_offset).z;
	if (depth + 0.003 < shadow_coord.z)
		return false;
	return true;
}

float PCFfactor() {
	float x, y;
	float sum = 0;
	for (y = -1.5; y <= 1.5; y += 1.0)
	  for (x = -1.5; x <= 1.5; x += 1.0)
		if (inLight(vec2(x, y)))
			sum += 1.0;
	return sum / 16;
}

float EvalShadow() {
	float visibility = 1.0; // not shadowed
	if (!inLight(vec2(0.0, 0.0))) {
		// visibility = 0.2;
		visibility = mix(0.2, 1.0, PCFfactor());
	}
	return visibility;
}



// Depth texture to linear space

vec4 depth() {
	float d = pow(texture(depth_texture, texture_coord).x , 256);
	return vec4(d, d, d, 1.0);
}