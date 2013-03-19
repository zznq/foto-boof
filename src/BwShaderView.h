#ifndef BOOF_BWSHADERVIEW
#define BOOF_BWSHADERVIEW

#include "View.h"
#include "KinectController.h"
#include "ShaderEffect.h"

class BwShaderView : public View {
public:
	BwShaderView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height) {
		addEffect(View::VisualEffectPtr(new ShaderEffect("bw_shader_effect", "shaders/default.vert", "shaders/grayscale.frag")));
	}
};

#endif