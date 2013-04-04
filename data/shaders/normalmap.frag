#version 110
uniform sampler2DRect tex;
const float xOffset = 1.0;
const float yOffset = 1.0;

void main()
{
	float sCoord = gl_TexCoord[0].s;
	float tCoord = gl_TexCoord[0].t;
	float depth  = texture2DRect( tex, vec2( sCoord, tCoord ) ).r;
	float leftDepth = texture2DRect( tex, vec2( sCoord + xOffset, tCoord ) ).r;
	float rightDepth = texture2DRect( tex, vec2( sCoord - xOffset, tCoord ) ).r;
	float upperDepth = texture2DRect( tex, vec2( sCoord, tCoord + yOffset ) ).r;
	float lowerDepth = texture2DRect( tex, vec2( sCoord, tCoord - yOffset ) ).r;

	vec3 normal = normalize( vec3( ( ( rightDepth - depth ) + ( depth - leftDepth ) ) * 0.5, 
								   ( ( lowerDepth - depth ) + ( depth - upperDepth ) ) * 0.5, 
								   0.01 ) );
							   
	gl_FragColor.rgb = normal;
	gl_FragColor.a = 1.0;
}






