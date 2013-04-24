uniform sampler2D depth_tex, normal_tex;
uniform float chub_factor;
uniform vec3 eyePos;

varying vec3 vNormal;
varying float depth;

void main()
{
	gl_TexCoord[0]		= gl_MultiTexCoord0;
	vec4 vVertex		= vec4( gl_Vertex );

	depth				= texture2D( depth_tex, gl_TexCoord[0].st ).r;
	vNormal				= texture2D( normal_tex, gl_TexCoord[0].st ).rgb;

	vVertex.xy			*= 2.5;
	vec3 dirToVertex	= normalize( eyePos - vVertex.xyz );

	//vVertex.xyz			+= depth * 500.0 * dirToVertex;
	//vVertex.xyz			+= vNormal * chub_factor * 3.0 * depth;
	vVertex.xyz += dirToVertex * vNormal * chub_factor;

	
	gl_Position			= gl_ModelViewProjectionMatrix * vVertex;
}
