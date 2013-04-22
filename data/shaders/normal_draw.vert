#version 120

uniform sampler2D normal_tex;
varying vec3 normal;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
    gl_Position = gl_Vertex;
    normal = texture2D(normal_tex, gl_TexCoord[0].st).rgb;
    gl_FrontColor = gl_Color;
}