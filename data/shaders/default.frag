#version 120

uniform sampler2DRect texture;

// simple pass through fragment shader
void main()
{
	gl_FragColor.rgb = texture2DRect(texture, gl_TexCoord[0].st).rgb;
	gl_FragColor.a = 1.0;
}