#version 150
 
layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform int cullBg = 1;
uniform float cullingValue = 0.1;
uniform float normal_length = 1.0;

in float depth[];
in vec3 normal[];
in mat4 mvp[];

out vec3 fcolor;

bool cullVertex(float depth)
{
	return (cullBg == 1) ? (depth < cullingValue) : (depth > cullingValue);
}

void main() {
	if (!cullVertex(depth[0]))
	{
		fcolor = vec3(0.0);
		gl_Position = mvp[0] * gl_in[0].gl_Position;
		EmitVertex();

		fcolor = vec3(1.0);
		gl_Position = mvp[0] * vec4(gl_in[0].gl_Position.xyz + normal[0] * depth[0] * normal_length, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}