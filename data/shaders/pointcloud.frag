#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect color_lookup;
uniform float near_depth;
uniform float far_depth;

in float depth;

void main()
{
	if (depth < near_depth || depth > far_depth)
	{
		discard;
	}

	//gl_TexCoord[0] = the current pixel location that this shader is processing
	vec4 color = texture2DRect(color_lookup, gl_TexCoord[0].st);
	
	//gl_FragColor = vec4(gl_PointCoord.st, 0, 1);
	gl_FragColor = color;
}