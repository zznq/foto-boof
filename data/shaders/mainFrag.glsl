#version 120
uniform sampler2D depth_tex, normal_tex, color_tex;
uniform float clip = 1.0;
uniform float brightness = 1.0;

varying vec3 vNormal;
varying float depth;

void main()
{
	if( depth < clip ) discard;
	vec3 colorFrag	= texture2D(color_tex, gl_TexCoord[0].st).rgb;
	colorFrag = texture2D(normal_tex, gl_TexCoord[0].st).rgb;
	gl_FragColor.rgb	= colorFrag * brightness;
	gl_FragColor.a		= 1.0;
}
