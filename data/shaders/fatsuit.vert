
uniform sampler2DRect normal_tex;
uniform float chubFactor = 1.0;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 normal = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	
	vec4 position = vec4(gl_Vertex);
	position.xyz += normal * chubFactor;
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;
}