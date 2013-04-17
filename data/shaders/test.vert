#version 130

uniform sampler2DRect texture;
uniform sampler2DRect normal_tex;
uniform float chub_factor = 100.0;

varying float depth;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec3 normal = texture2DRect(normal_tex, gl_Vertex.xy).rgb;
	
	vec4 position = vec4(gl_Vertex.xyz + chub_factor*normal, 1.0);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;

	// set depth variable to pass to next stage
	depth = gl_Position.z;
}