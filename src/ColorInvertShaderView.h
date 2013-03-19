#ifndef BOOF_COLORINVERTSHADERVIEW
#define BOOF_COLORINVERTSHADERVIEW

#include "View.h"
#include "KinectController.h"
#include "ShaderEffect.h"

class ColorInvertShaderView : public View {
public:
	ColorInvertShaderView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height) {
		addEffect(View::VisualEffectPtr(new ShaderEffect("color_invert_shader_effect", "shaders/default.vert", "shaders/colorinvert.frag")));
	}
};

#endif