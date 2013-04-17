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
	
	gl_FragColor = gl_Color;
	//gl_FragColor = texture2DRect(color_tex, vec2(gl_TexCoord[0])); 
	//gl_FragColor = texture2DRect(normal_tex, gl_FragCoord.st);
	//gl_FragColor.a = 1.0;
	
}