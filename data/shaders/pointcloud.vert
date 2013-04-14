#version 120

// default vertex shader
//
// Generic vertex transformation,
// copy primary color

out float depth;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;

	// set depth variable to pass to next stage
	depth = gl_Position.z;
}