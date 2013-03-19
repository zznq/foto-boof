#version 120
#extension GL_ARB_texture_rectangle : enable
// grayscale.fs
//
// convert RGB to grayscale

uniform sampler2DRect texture;

void main()
{
	// get color from texture coordinate
	vec4 color = texture2DRect(texture, gl_TexCoord[0].st);

	// Convert to grayscale using NTSC conversion weights
	float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    // replicate grayscale to RGB components
    gl_FragColor = vec4(gray, gray, gray, color.a);
}