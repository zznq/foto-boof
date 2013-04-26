#version 130

uniform sampler2D depth_tex;

in vec2 texCoord;

void main()
{
	vec4 colorFrag = texture2D(depth_tex, texCoord);
	
	//if( colorFrag.a < clip ) discard;
	//colorFrag = texture2D(depth_tex, texCoord).rgb;
	//colorFrag = vec4(texture2D(normal_tex, texCoord).rgb, 1.0);
	gl_FragColor.rgb = colorFrag.rgb;
	gl_FragColor.a = 1.0;
}