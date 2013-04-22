#version 120       
#extension GL_ARB_geometry_shader4 : enable

uniform float draw_length;     

varying vec3 normal[];      

void main()
{
    // assert(gl_VerticesIn == 3);
    // assert(GL_GEOMETRY_INPUT_TYPE_EXT == GL_TRIANGLES);
    // assert(GL_GEOMETRY_OUTPUT_TYPE_EXT == GL_LINE_STRIP);
    // assert(GL_GEOMETRY_VERTICES_OUT_EXT == 6);      
 
    for(int i = 0; i < gl_VerticesIn; ++i)
    {
        gl_Position = gl_ModelViewProjectionMatrix * gl_PositionIn[i];
        gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex();
 
        gl_Position = gl_ModelViewProjectionMatrix * (gl_PositionIn[i] + (vec4(normal[i], 0) * draw_length));
        gl_FrontColor = gl_FrontColorIn[i];
        EmitVertex();      
 
        EndPrimitive();
    }
}