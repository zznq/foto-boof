#version 130

// default vertex shader
//
// Generic vertex transformation,
// copy primary color

uniform sampler2D depth_tex;
out float depth;
out vec2 texCoordVs;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;
	
	depth = texture2D(depth_tex, gl_TexCoord[0].st).a;
	texCoordVs = gl_TexCoord[0].st;
}