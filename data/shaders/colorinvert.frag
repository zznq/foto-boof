#version 120

// colorinvert.fs
//
// invert like a color negative

void main()
{
   // invert color components
   gl_FragColor.rgb = 1.0 - gl_Color.rgb;
   gl_FragColor.a = 1.0;
}