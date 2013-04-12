
uniform sampler2DRect normal_tex;
uniform float chubFactor = 1.0;
in vec3 in_Position;

void main() 
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 normal = texture2DRect(normal_tex, gl_TexCoord[0].st).rgb;
	
	//vec4 position = vec4(gl_Vertex);
	vec4 position = vec4(in_Position, 1.0);
	position.xyz += normal * chubFactor;
	//vec4 position = vec4(gl_Vertex.xy + gl_Normal.xy * chubFactor, gl_Vertex.z, 1.0);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
	gl_FrontColor = gl_Color;
}