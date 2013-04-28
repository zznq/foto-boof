#ifndef BOOF_COLORDEPTHSHADERVIEW
#define BOOF_COLORDEPTHSHADERVIEW

#include "View.h"
#include "KinectController.h"
#include "ColorDepthShaderEffect.h"

class ColorDepthShaderView : public View {
public:
	ColorDepthShaderView(KinectControllerPtr kinectController, int width, int height) : View(kinectController, width, height, true) {
		addEffect(View::VisualEffectPtr(new ColorDepthShaderEffect("color_invert_shader_effect", width, height, "shaders/default.vert", "shaders/colordepth.frag")));
	}

	void setup()
	{
		m_canvas->setVisible(false);
	}
};

#endif