#version 120

uniform sampler2DRect tex;
uniform float progress_value;

// simple pass through fragment shader
void main()
{
	vec4 tex_color = texture2DRect(tex, vec2(gl_TexCoord[0]));

	tex_color.a *= progress_value;
	
	gl_FragColor = tex_color;	
}