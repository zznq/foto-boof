#version 130

uniform sampler2D depth_tex, normal_tex;
uniform float chub_factor;
uniform vec3 eyePos;

out float depth;
out vec2 texCoordVs;
out vec4 vpeyeVs;
out vec4 vneyeVs;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec3 vertexNormal = texture2D( normal_tex, gl_TexCoord[0].st ).rgb * 2.0 - 1.0;
	vec4 vertexPosition = vec4(gl_Vertex.xyz, 1.0);
	
	//vec4 vVertex = vec4( gl_Vertex );

	depth = texture2D( depth_tex, gl_TexCoord[0].st ).a;
	//vec3 vNormal = texture2D( normal_tex, gl_TexCoord[0].st ).rgb * 2.0 - 1.0;

	vec3 dirToVertex = normalize( eyePos - vertexPosition.xyz );
	
	//vertexPosition.xy *= 2.5;
	//vertexPosition.xyz += depth * dirToVertex;
	vertexPosition.xyz += gl_NormalMatrix * vertexNormal * chub_factor /** 3.0 * depth*/;

	gl_Position = gl_ModelViewProjectionMatrix * vertexPosition;
	
	texCoordVs = gl_TexCoord[0].st;
	
	vpeyeVs = gl_ModelViewMatrix * vertexPosition;
	vneyeVs = vec4(gl_NormalMatrix * vertexNormal, 0);
}
