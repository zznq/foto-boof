#version 120

// colorinvert.fs
// invert like a color negative

uniform sampler2DRect texture;
uniform sampler2DRect color_lookup;

void main()
{
	//gl_TexCoord[0] = the current pixel location that this shader is processing
	float greyness = texture2DRect(texture, gl_TexCoord[0].st).r;
	
	//If you bind an integer texture type to a float sampler, it will generally get rescaled to be in the range [-1.0...1.0]
	ivec2 pos = ivec2(int(greyness * 255), 0);
	vec4 clu = texture2DRect(color_lookup, pos);	
	
	// Make extremes white
	if(greyness * 255 == 0) {
		clu = vec4(255.0,255.0,255.0,255.0);
	}
	
	// invert color components
	gl_FragColor.rgb = clu.rgb;
}