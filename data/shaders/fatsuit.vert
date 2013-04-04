#version 120

uniform sampler2DRect normalTexture;
float chubFactor = 10.0;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 normal = texture2DRect(normalTexture, gl_TexCoord[0].st).rgb;
	
	vec4 position = vec4(gl_Vertex);
	position.xyz += normal.xyz * chubFactor;
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;
}