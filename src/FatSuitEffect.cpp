#include "NormalMapEffect.h"

#include "FatSuitEffect.h"
#include "View.h"
#include "KinectController.h"
#include "KinectData.h"

FatSuitEffect::FatSuitEffect(int width, int height)
: ShaderEffect("fat_suit", "shaders/fatsuit.vert")
{

}

FatSuitEffect::~FatSuitEffect() 
{

}

void FatSuitEffect::addUI(CanvasPtr canvas) 
{

}

void FatSuitEffect::guiEvent(ofxUIEventArgs &e)
{

}