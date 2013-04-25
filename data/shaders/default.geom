#version 150
 
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform int cullBg = 1;
uniform float cullingValue = 0.1;
in float depth[];
in vec2 texCoordVs[];

out vec2 texCoord;

bool cullVertex(float depth)
{
	return (cullBg == 1) ? (depth < cullingValue) : (depth > cullingValue);
}

void main()
{
	gl_Position = vec4(0);
    for(int i = 0; i < gl_in.length(); ++i)
    {
		if (!cullVertex(depth[i]))
		{
			gl_Position = gl_in[i].gl_Position;
			texCoord = texCoordVs[i];
			EmitVertex();
		}
    }
    EndPrimitive();
}