#version 130

uniform sampler2DRect texture;
uniform sampler2DRect normal_tex;

uniform float near_depth;
uniform float far_depth;

varying float depth;

// simple pass through fragment shader
void main()
{
	if (depth < near_depth || depth > far_depth)
	{
		discard;
	}
	
	//gl_FragColor = gl_Color;
	gl_FragColor.rgb = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	//gl_FragColor.rgb = texture2DRect(texture, gl_TexCoord[0].st).rgb;
	//gl_FragColor.a = 1.0;
	
}