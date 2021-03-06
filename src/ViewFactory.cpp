
#include "ViewFactory.h"
#include "KinectController.h"
#include "View.h"
#include "BwView.h"
#include "BwShaderView.h"
#include "ColorInvertShaderView.h"
#include "ColorDepthShaderView.h"
#include "MeshView.h"
#include "NormalMapView.h"
#include "FatSuitView.h"
#include "OutlineView.h"
#include "IdleView.h"
#include "FatSuitWireView.h"
#include "TestView.h"

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
		case ViewType::NormalMapView:
			ptr.reset(new NormalMapView(kinectController, width, height));
			break;
		case ViewType::FatSuitView:
			ptr.reset(new FatSuitView(kinectController, width, height));
			break;
		case ViewType::PointCloudView:
			ptr.reset(new MeshView(kinectController, width, height, true));
			break;
		case ViewType::OutlineView:
			ptr.reset(new OutlineView(kinectController, width, height));
			break;
		case ViewType::Idle:
			ptr.reset(new IdleView(kinectController, width, height));
			break;
		case ViewType::TestView:
			ptr.reset(new TestView(kinectController, width, height));
			break;
		case ViewType::FatSuitWireView:
			ptr.reset(new FatSuitWireView(kinectController, width, height));
			break;
		default:
			ptr.reset(new View(kinectController, width, height));
			break;
	}

	return ptr;
}