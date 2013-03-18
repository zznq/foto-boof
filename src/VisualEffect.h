#ifndef PIXEL_EFFECT_H
#define PIXEL_EFFECT_H

#include <string>

#include "ofPixels.h"

class View;

// serves as a base class for all effects
class VisualEffect {
public:
	VisualEffect(const std::string& name);
	virtual ~VisualEffect();
	
	const std::string& getName();
	void setParent(View* parent);

	// override these to customize effect behavior
	virtual void preDraw();
	virtual void draw();
	virtual void postDraw();

protected:
	std::string m_name;
	View* m_parent;
};

#endif