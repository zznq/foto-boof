#ifndef PIXEL_EFFECT_H
#define PIXEL_EFFECT_H

#include <string>

#include "View.h"
#include "ofxUI.h"
#include "ofPixels.h"

class View;

// serves as a base class for all effects
class VisualEffect {
public:
	typedef std::vector<ofxUIWidget *> Widgets;
	typedef ofPtr<ofxUICanvas> CanvasPtr;
public:
	VisualEffect(const std::string& name);
	virtual ~VisualEffect();
	
	const std::string& getName();
	void setParent(View* parent);

	virtual void addUI(CanvasPtr canvas);
	virtual void removeUI(CanvasPtr canvas);

	// override these to customize effect behavior
	virtual void preDraw();
	virtual void draw();
	virtual void postDraw();

protected:
	Widgets m_widgets;
	ofEvent<ofxUIEventArgs>* m_event;

	std::string m_name;
	View* m_parent;

	virtual void guiEvent(ofxUIEventArgs &e);
};

#endif