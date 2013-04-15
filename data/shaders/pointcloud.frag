#version 130

uniform sampler2DRect color_lookup;
uniform float near_depth;
uniform float far_depth;

varying float depth;
flat in float index;

void main()
{
	if (depth < near_depth || depth > far_depth)
	{
		discard;
	}

	//gl_TexCoord[0] = the current pixel location that this shader is processing
	//vec4 color = texture2DRect(color_lookup, vec2(index/100*640, index/100*480));
	//vec4 color = texture2DRect(color_lookup, );
	
	//gl_FragColor = vec4(gl_PointCoord.st, 0, 1);
	gl_FragColor = gl_Color;
}