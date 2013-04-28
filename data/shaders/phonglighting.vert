#version 130

uniform sampler2D normal_tex;
 
out vec4 vpeye;
out vec4 vneye;
 
void main() 
{
	vec3 vertexNormal = texture2D( normal_tex, gl_TexCoord[0].st ).rgb * 2.0 - 1.0;
	vec4 vertexPosition = vec4(gl_Vertex.xyz, 1.0);
	
	vpeye = gl_ModelViewMatrix * vertexPosition;
	vneye = vec4(gl_NormalMatrix * vertexNormal, 0);
	gl_Position = gl_ModelViewProjectionMatrix * vertexPosition;
}