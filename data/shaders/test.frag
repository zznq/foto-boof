#version 130

uniform sampler2DRect color_tex;
uniform sampler2DRect normal_tex;

uniform float near_depth;
uniform float far_depth;

varying float depth;

void main()
{
	if (depth < near_depth || depth > far_depth)
	{
		discard;
	}
	
	//gl_FragColor = gl_Color;
	gl_FragColor.rgb = texture2DRect(color_tex, gl_TexCoord[0].st).rgb;
	gl_FragColor.rgb = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	//gl_FragColor.a = 1.0;
	
}