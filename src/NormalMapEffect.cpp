#include "NormalMapEffect.h"

#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

NormalMapEffect::NormalMapEffect(int width, int height)
: ShaderEffect("normal_map", "shaders/default.vert", "shaders/normalmapv2.frag")
{

}

NormalMapEffect::~NormalMapEffect() 
{

}

void NormalMapEffect::addUI(CanvasPtr canvas) 
{

}

void NormalMapEffect::guiEvent(ofxUIEventArgs &e)
{

}