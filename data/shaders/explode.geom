#version 150

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices = 3) out;

uniform int cullBg = 1;
uniform float cullingValue = 0.1;
uniform float time;

in mat4 projection[];
in mat4 modelview[];
in vec3 normalVs[];
in vec2 texCoordVs[];
in float depth[];

out vec3 position_eye;
out vec3 n;
out vec2 texCoord;

bool cullVertex(float depth)
{
	return (cullBg == 1) ? (depth < cullingValue) : (depth > cullingValue);
}

void main() {
  vec3 a = vec3(gl_in[1].gl_Position - gl_in[0].gl_Position);
  vec3 b = vec3(gl_in[2].gl_Position - gl_in[0].gl_Position);

  vec3 center = vec3(gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3.0;
  //vec3 normal = normalize(cross(b, a));
  vec3 normal = (normalVs[0] + normalVs[1] + normalVs[2]) / 3.0;
  vec3 new_center = normal * time + center;
  new_center.y += -0.1 * time * time;

  vec3 diff = new_center - center;

  for (int i = 0; i < gl_in.length(); ++i) {
	if (!cullVertex(depth[i]))
	{
		position_eye = (modelview[0] * (gl_in[i].gl_Position + vec4(diff, 0.0))).xyz;
		n = normalVs[i];
		gl_Position = projection[0] * vec4(position_eye, 1.0);
		texCoord = texCoordVs[i];
		EmitVertex();
	}
  }

  EndPrimitive();
}