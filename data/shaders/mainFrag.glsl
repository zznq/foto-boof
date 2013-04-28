#version 130
uniform sampler2D depth_tex, normal_tex;
uniform float clip = 1.0;
uniform int useNormalColor = 0;

in vec2 texCoord;

void main()
{
	float brightness = 1.5;
	vec4 colorFrag = texture2D(depth_tex, texCoord);
	
	//if( colorFrag.a < clip ) discard;
	colorFrag = vec4(0, 0, 0, 0);
	if (useNormalColor == 1) {
		colorFrag = vec4(texture2D(normal_tex, texCoord).rgb, 1.0);
		brightness = 2.0;
	}
	else {
		colorFrag = vec4(texture2D(depth_tex, texCoord).rgba);
	}

	gl_FragColor.rgb = colorFrag.rgb * brightness;
	gl_FragColor.a = 1.0;
}
