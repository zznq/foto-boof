#version 130

uniform sampler2D depth_tex, normal_tex;
uniform float chub_factor;
uniform vec3 eyePos;

out float depth;

out vec2 texCoordVs;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 vVertex = vec4( gl_Vertex );

	depth = texture2D( depth_tex, gl_TexCoord[0].st ).a;
	vec3 vNormal = texture2D( normal_tex, gl_TexCoord[0].st ).rgb * 2.0 - 1.0;

	vec3 dirToVertex = normalize( eyePos - vVertex.xyz );
	
	//vVertex.xy *= 2.5;
	vVertex.xyz	+= depth * dirToVertex;
	vVertex.xyz += vNormal * chub_factor * 3.0 * depth;

	gl_Position = gl_ModelViewProjectionMatrix * vVertex;
	
	texCoordVs = gl_TexCoord[0].st;
}
