
uniform sampler2DRect texture;
uniform sampler2DRect normal_tex;

// simple pass through fragment shader
void main()
{
	gl_FragColor.rgb = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	//gl_FragColor.rgb = texture2DRect(texture, gl_TexCoord[0].st).rgb;
	gl_FragColor.a = 1.0;
}