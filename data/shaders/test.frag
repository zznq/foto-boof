#version 130

uniform sampler2DRect color_tex;
uniform sampler2DRect normal_tex;

uniform float near_depth;
uniform float far_depth;
uniform vec2 normal_size;

varying float depth;

void main()
{
	if (depth < near_depth || depth > far_depth)
	{
		discard;
	}
	
	//vec3 normal = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	
    //vec2 coord = gl_FragCoord.xy/normal_size ;
    //coord.y = 1.0f-coord.y ;
	
	gl_FragColor = gl_Color;
	//gl_FragColor = texture2DRect(color_tex, vec2(gl_TexCoord[0])); 
	//gl_FragColor = texture2DRect(normal_tex, coord);
	//gl_FragColor = vec4(normal, 1.0);
	//gl_FragColor.a = 1.0;
	
}