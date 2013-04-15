#version 130

uniform sampler2DRect texture;
uniform sampler2DRect normal_tex;
uniform float chub_factor = 1.0;
in vec3 in_Position;

varying float depth;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 normal = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	
	//vec4 position = vec4(gl_Vertex);
	//vec4 position = vec4(normal, 1.0);
	vec4 position = vec4(in_Position + normal*chub_factor, 1.0);
	//vec4 position = vec4(gl_Vertex.xy + gl_Normal.xy * chub_factor, gl_Vertex.z, 1.0);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;

	// set depth variable to pass to next stage
	depth = gl_Position.z;
}