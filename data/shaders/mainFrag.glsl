#version 130

uniform sampler2D depth_tex, normal_tex;
uniform float clip = 1.0;
uniform float brightness = 1.0;
uniform int useNormalColor = 0;

uniform vec4 lightPosition; // light position in eye coords
uniform vec4 Ka = vec4(3, 3, 3, 1); // ambient coefficient
uniform vec4 Kd = vec4(1, 1, 1, 1); // diffuse coefficient
uniform vec4 Ks = vec4(1, 1, 1, 1); // specular coefficient
uniform float Ns = 1.0; // specular exponent
uniform vec4 Ld; // diffuse light colour

in vec4 vpeye; // fragment position in eye coords
in vec4 vneye; // surface normal in eye coords
in vec2 texCoord;

void main()
{
	vec4 colorFrag = texture2D(depth_tex, texCoord);
	
	//if( colorFrag.a < clip ) discard;
	colorFrag = vec4(0, 0, 0, 0);
	if (useNormalColor == 1) {
		colorFrag = vec4(texture2D(normal_tex, texCoord).rgb, 1.0);
	}
	else {
		colorFrag = vec4(texture2D(depth_tex, texCoord).rgba);
	}

	//gl_FragColor.rgb = colorFrag.rgb * brightness;
	//gl_FragColor.a = 1.0;
	
	// phong lighting
	vec4 n_eye = normalize(vneye); // normalise just to be on the safe side
	vec4 s_eye = normalize(lightPosition - vpeye); // get direction from surface fragment to light
	vec4 v_eye = normalize(-vpeye); // get direction from surface fragment to camera
	vec4 h_eye = normalize(v_eye + s_eye); // Blinn's half-way vector
	//vec4 r = reflect(-s, vneye); // Phong's full reflection (could use instead of h)

	vec4 Ia = vec4(0.1,0.1,0.1,1) * Ka; // ambient light has a hard-coded colour here, but we could load an La value
	vec4 Id = colorFrag * Kd * max(dot(s_eye, n_eye), 0.0); // max() is a safety catch to make sure we never get negative colours
	vec4 Is = vec4(1,1,1,1) * Ks * pow(max(dot(h_eye, n_eye), 0), Ns); // my ambient light colour is hard coded white, but could load Ls

	gl_FragColor = (Ia + Id + Is);
}
