#ifndef VIEW_FACTORY_H
#define VIEW_FACTORY_H

#include "ofTypes.h"

class KinectController;
typedef ofPtr<KinectController> KinectControllerPtr;

class View;
typedef ofPtr<View> ViewPtr;

struct ViewType {
	enum Enum {
		Normal = 0,
		Idle,
		BwView,
		BwShaderView,
		ColorInvertShaderView,
		ColorDepthShaderView,
		MeshView,
		NormalMapView,
		FatSuitView,
		PointCloudView,

		// add any new view types above here
		Max,
		Invalid = -1
	};
};

ViewPtr CreateView(const ViewType::Enum& type, const KinectControllerPtr& kinectController, int width, int height);

#endif