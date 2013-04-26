#version 130

uniform sampler2D depth_tex, normal_tex;
uniform float chub_factor;
uniform vec3 eyePos;

out float depth;
out vec3 normal;
out mat4 mvp;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	depth = texture2D( depth_tex, gl_TexCoord[0].st ).a;
    normal = normalize( texture(normal_tex, gl_TexCoord[0].st).xyz * 2.0 - 1.0);
    gl_FrontColor = gl_Color;
	
	vec4 vVertex = vec4(gl_Vertex);

	vec3 dirToVertex = normalize( eyePos - vVertex.xyz );
	
	//vVertex.xy *= 2.5;
	//vVertex.xyz	+= depth * 500.0 * dirToVertex;
	vVertex.xyz += normal * chub_factor * 3.0 * depth;
	
	gl_Position = vVertex;
	
	mvp = gl_ProjectionMatrix * gl_ModelViewMatrix;
}