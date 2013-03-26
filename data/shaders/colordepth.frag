#version 120

// colorinvert.fs
//
// invert like a color negative

uniform sampler2DRect texture;
//uniform sampler2DRect color_lookup;

// Change this to a look up table!
vec3 hsv2rgb(vec3 hsv)
{
    float h = hsv.x * 6.; /* H in 0°=0 ... 1=360° */
    float s = hsv.y;
    float v = hsv.z;
    float c = v * s;

    vec2 cx = vec2(v*s, c * ( 1 - abs(mod(h, 2.)-1.) ));

    vec3 rgb = vec3(0., 0., 0.);
    if( h < 1. ) {
        rgb.rg = cx;
    } else if( h < 2. ) {
        rgb.gr = cx;
    } else if( h < 3. ) {
        rgb.gb = cx;
    } else if( h < 4. ) {
        rgb.bg = cx;
    } else if( h < 5. ) {
        rgb.br = cx;
    } else {
        rgb.rb = cx;
    }
    return rgb + vec3(v-cx.y);
}

void main()
{
	// get color from texture coordinate
	vec4 color = texture2DRect(texture, gl_TexCoord[0].st);
	//vec4 clu = texture2DRect(color_lookup, gl_TextCoord[color.r].st);

	// invert color components
	gl_FragColor.rgb = hsv2rgb(vec3(color.r, 255, 255));
}