#version 120

uniform sampler2DRect active;
uniform sampler2DRect inactive;
uniform float blend_level;

// simple pass through fragment shader
void main()
{
	vec4 tex_active = texture2DRect(active, vec2(gl_TexCoord[0]));
	vec4 tex_inactive = texture2DRect(inactive, vec2(gl_TexCoord[0]));

	// INTERPOLATE
	gl_FragColor = mix(tex_inactive, tex_active, blend_level);	
}