#version 410

#define Diffuse	u_texture_0
#define W_Pos	u_texture_1
#define W_Norm	u_texture_2
#define V_Pos	u_texture_3
#define V_Norm	u_texture_4
#define Noise1	u_texture_5
#define Noise2	u_texture_6
#define DepthT	u_texture_7

uniform sampler2D u_texture_0;	// Diffuse texture
uniform sampler2D u_texture_1;	// World positions
uniform sampler2D u_texture_2;	// World normals
uniform sampler2D u_texture_3;	// View positions
uniform sampler2D u_texture_4;	// View normals
uniform sampler2D u_texture_5;	// Random Texture 1
uniform sampler2D u_texture_6;	// Random Texture 2
uniform sampler2D u_texture_7;	// Depth buffer

uniform vec2 resolution;
uniform int kernel_size;
uniform vec3 kernel[64];
uniform float u_rad;

uniform mat4 Projection;

layout(location = 0) out vec3 out_occlusion;

vec3 pSphere[16] = vec3[](vec3(0.53812504, 0.18565957, -0.43192),vec3(0.13790712, 0.24864247, 0.44301823),vec3(0.33715037, 0.56794053, -0.005789503),vec3(-0.6999805, -0.04511441, -0.0019965635),vec3(0.06896307, -0.15983082, -0.85477847),vec3(0.056099437, 0.006954967, -0.1843352),vec3(-0.014653638, 0.14027752, 0.0762037),vec3(0.010019933, -0.1924225, -0.034443386),vec3(-0.35775623, -0.5301969, -0.43581226),vec3(-0.3169221, 0.106360726, 0.015860917),vec3(0.010350345, -0.58698344, 0.0046293875),vec3(-0.08972908, -0.49408212, 0.3287904),vec3(0.7119986, -0.0154690035, -0.09183723),vec3(-0.053382345, 0.059675813, -0.5411899),vec3(0.035267662, -0.063188605, 0.54602677),vec3(-0.47761092, 0.2847911, -0.0271716));

const float far_plane = 5000.0;
const float near_plane = 0.1;
vec4 linearDepth(sampler2D depthTexture, vec2 coord) {
	float dz = texture(depthTexture, coord).x;
	float z = (2.0 * near_plane) / 
		(far_plane + near_plane - dz * (far_plane - near_plane));
	return vec4(z, z, z, 1.0);
}

void main() {

	// -------------------------------------------------------------------------
	// Fragment properties
	
	vec2 text_coord = gl_FragCoord.xy / resolution;
	vec4 diffuse = texture(Diffuse, text_coord);
	vec3 pNormal = texture(V_Norm, text_coord).xyz;
	vec3 pPos = texture(V_Pos, text_coord).xyz;
	float pDepth = linearDepth(DepthT, text_coord).x;
	
	// -------------------------------------------------------------------------
	// Random Noise vector
	
	vec2 noise_scale = resolution / 4.0;
	vec3 fres = normalize(texture(Noise2, text_coord * noise_scale).xyz);

	/////////////////////
	
	// vec3 rvec = fres;
	// vec3 tangent = normalize(rvec - pNormal * dot(rvec, pNormal));
	// vec3 bitangent = cross(pNormal, tangent);
	// mat3 tbn = mat3(tangent, bitangent, pNormal);

	/////////////
	
	// -------------------------------------------------------------------------
	
	const float strength = 0.1;
	const float falloff = 0.00002;
	const float rad = 0.0006;
	float radD = rad/pDepth;	
	float ao = 0.0;
 	
	// -------------------------------------------------------------------------
	
	for(int i = 0; i < kernel_size; i++)
	{
		////////////////////
		// IQ SSAO
		
		vec3 ray = u_rad * reflect(kernel[i], fres);
		vec3 se = pPos + sign(dot(ray, pNormal)) * ray;
		// se = tbn * pSphere[i];
		// se = pPos + ray;
		
		// Compute clip space coordinates
		vec4 sp = Projection * vec4(se, 1.0);
		sp.xy /= sp.w;
		sp.xy = sp.xy * 0.5 + vec2(0.5);
		
        float sd = linearDepth(DepthT, sp.xy).x;
		float dd = pDepth - sd; 

		// IQ attenuation	
		// float rangeCheck = dd > 0.004 ? 1.0 : 0.0;
		// float zd = rangeCheck * 20.0 * max(dd, 0.0 );
        // ao += 1.0 / (1.0 + zd * zd);
		// ao += rangeCheck;
		
		// calculate the difference between the normals as a weight
		float depthDifference = pDepth - sd;
		if (abs(depthDifference) <= 0.004) {
			vec3 sn = texture(V_Norm, sp.xy).xyz;
			float normDiff = (1.0 - dot(sn, pNormal));
			// the falloff equation, starts at falloff and is kind of 1/x^2 falling
			ao += step(falloff, depthDifference) * normDiff * (1.0 - smoothstep(falloff, strength, depthDifference));		
		}
	// -------------------------------------------------------------------------
   }
	ao = 1.0 - ao / kernel_size;
	
	out_occlusion = vec3(ao);
	// out_occlusion = vec3(1.0, 0.0, 0.0);
}
