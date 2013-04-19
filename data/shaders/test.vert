#version 130

uniform sampler2D texture;
uniform sampler2D normal_tex;
uniform float chub_factor = 1.0;

varying float depth;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	// Extract the normal from the normal map,
	// convert [0,1] range to [-1,1] 
	vec3 normal = normalize(texture2D(normal_tex, gl_TexCoord[0].st).rgb * 2.0 - 1.0);
	
	//float df = 0.30*normal.x + 0.59*normal.y + 0.11*normal.z;
	
	vec4 position = vec4(gl_Vertex.xyz + chub_factor*normal, 1.0);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;
	
	// set depth variable to pass to next stage
	depth = gl_Position.z;
}