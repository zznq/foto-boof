
#include "ViewFactory.h"
#include "KinectController.h"
#include "View.h"
#include "BwView.h"
#include "BwShaderView.h"
#include "ColorInvertShaderView.h"
#include "ColorDepthShaderView.h"
#include "MeshView.h"

ViewPtr CreateView(const ViewType::Enum& type, const KinectControllerPtr& kinectController, int width, int height) {
	ViewPtr ptr;
	switch (type) {
		case ViewType::Normal:
			ptr.reset(new View(kinectController, width, height)); 
			break;
		case ViewType::BwView:
			ptr.reset(new BwView(kinectController, width, height));
			break;
		case ViewType::BwShaderView:
			ptr.reset(new BwShaderView(kinectController, width, height));
			break;
		case ViewType::ColorInvertShaderView:
			ptr.reset(new ColorInvertShaderView(kinectController, width, height));
			break;
		case ViewType::ColorDepthShaderView:
			ptr.reset(new ColorDepthShaderView(kinectController, width, height));
			break;
		case ViewType::MeshView:
			ptr.reset(new MeshView(kinectController, width, height));
			break;
		default:
			ptr.reset(new View(kinectController, width, height));
			break;
	}

	return ptr;
}