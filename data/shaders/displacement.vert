uniform sampler2D displacement_tex;
uniform sampler2D normal_tex;
uniform sampler2D color_tex;

uniform float chub_factor = 1.0;

varying vec4 vertexPos;
varying float depth;

void main(void)
{
	vec4 newVertexPos;
	vec4 dv;
	vec3 nm;
	float df;
	
	vertexPos = gl_Vertex;
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	dv = texture2D(displacement_tex, gl_Vertex.xy);
	//nm = texture2D(normal_tex, gl_Vertex.xy).xyz;
	nm = normalize(texture2D(normal_tex, gl_Vertex.xy).rgb * 2.0 - 1.0);
	
	df = 0.30*dv.x + 0.59*dv.y + 0.11*dv.z;
	
	newVertexPos = vec4(nm * chub_factor, 1.0) + gl_Vertex;
	
	//norm = normalize(gl_NormalMatrix * ((gl_Normal+nm)/2.0));
	gl_FrontColor = gl_Color;
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPos /** vec4(1.0,-1.0, 1.0,1.0)*/;
	
	depth = gl_Position.z;
}