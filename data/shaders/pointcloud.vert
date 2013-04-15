#version 130

// default vertex shader
//
// Generic vertex transformation,
// copy primary color

uniform float point_size = 1.0;
varying float depth;
flat out float index;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;

	// set point size
	gl_PointSize = point_size;
	
	// set depth variable to pass to next stage
	depth = gl_Position.z;
	
	// set index
	index = gl_VertexID;
}