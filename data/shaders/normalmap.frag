#version 110

uniform sampler2D tex;
uniform float xOffset;
uniform float yOffset;
uniform float scaleFactor;

void main()
{
	float sCoord = gl_TexCoord[0].s;
	float tCoord = gl_TexCoord[0].t;
	float depth  = texture2D( tex, vec2( sCoord, tCoord ) ).r;
	float leftDepth = texture2D( tex, vec2( sCoord + xOffset, tCoord ) ).r;
	float rightDepth = texture2D( tex, vec2( sCoord - xOffset, tCoord ) ).r;
	float upperDepth = texture2D( tex, vec2( sCoord, tCoord + yOffset ) ).r;
	float lowerDepth = texture2D( tex, vec2( sCoord, tCoord - yOffset ) ).r;

	//vec3 normal = normalize( vec3( ( ( rightDepth - depth ) + ( depth - leftDepth ) ) * 0.5, 
	//							   ( ( lowerDepth - depth ) + ( depth - upperDepth ) ) * 0.5, 
	//							   0.1 ) );
	
	float xAxis = scaleFactor * (leftDepth - rightDepth);
	float yAxis = scaleFactor * (upperDepth - lowerDepth);
	
	vec3 a = vec3(1, 0, xAxis);
	vec3 b = vec3(0, 1, yAxis);
	vec3 normal = cross(a, b) * 0.5 + 0.5;
	//vec3 normal = (vec3(b.z, a.z, 1.0) * 0.5) + 0.5;
	
	gl_FragColor.rgb = normal;
	gl_FragColor.a = 1.0;
}






