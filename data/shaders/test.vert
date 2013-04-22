#version 130

uniform sampler2D texture;
uniform sampler2D normal_tex;
uniform sampler2D displacement_tex;
uniform float chub_factor = 1.0;

varying float depth;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	// Extract the normal from the normal map,
	// convert [0,1] range to [-1,1] 
	vec3 normal = normalize(texture2D(normal_tex, gl_TexCoord[0].st).rgb * 2.0 - 1.0);
	
	//vec4 position = vec4(gl_Vertex.xyz + chub_factor*normal, 1.0);
	
	/*
	float displacement = texture2D(displacement_tex, gl_TexCoord[0].st).r;
	vec4 position = gl_Vertex;
	position.x += (gl_Normal.x + displacement) * chub_factor;
	position.y += (gl_Normal.y + displacement) * chub_factor;
	position.z += (gl_Normal.z + displacement) * chub_factor;
	*/
	

	vec4 bumpColor = texture2D(displacement_tex, gl_TexCoord[0].st) * 2.0 - 1.0;
	float df = 0.30*bumpColor.x + 0.59*bumpColor.y + 0.11*bumpColor.z;
    vec4 position = vec4(normal * df * chub_factor, 0.0) + gl_Vertex;

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;
	
	// set depth variable to pass to next stage
	depth = gl_Position.z;
}