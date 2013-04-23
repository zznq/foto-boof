#version 120

uniform sampler2D color_tex;
uniform sampler2D normal_tex;
uniform sampler2D displacement_tex;

uniform float near_depth;
uniform float far_depth;
uniform vec2 normal_size;

varying vec4 vertexPos;
varying float depth;
varying vec4 displacement;

void main()
{
	if (depth < near_depth || depth > far_depth)
	{
		discard;
	}
	
	vec4 dv = texture2D(displacement_tex, gl_TexCoord[0].st);
	if (dv.r < 0.1)
	{
		discard;
	}
	
	vec3 normal = texture2D(normal_tex, gl_TexCoord[0].st).rgb;
	
    //vec2 coord = gl_FragCoord.xy/normal_size ;
    //coord.y = 1.0f-coord.y ;
	
	//gl_FragColor = texture2D(displacement_tex, gl_TexCoord[0].st);
	//gl_FragColor = gl_Color;
	//gl_FragColor = vec4(normal,1.0);
	//gl_FragColor = texture2D(color_tex, gl_TexCoord[0].st); 
	//gl_FragColor = texture2D(normal_tex, coord);
	//gl_FragColor = vec4(normal, 1.0);
	//gl_FragColor.a = 1.0;
	//gl_FragData[0] = displacement;
	
}