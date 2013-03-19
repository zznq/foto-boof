#version 120

// colorinvert.fs
//
// invert like a color negative

uniform sampler2DRect texture;

void main()
{
	// get color from texture coordinate
	vec4 color = texture2DRect(texture, gl_TexCoord[0].st);

	// invert color components
	gl_FragColor.rgb = 1.0 - color.rgb;
	gl_FragColor.a = color.a;
}