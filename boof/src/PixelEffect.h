#include "ofPixels.h"

// serves as a base class for all effects
class PixelEffect {
public:
	PixelEffect(const std::string& name) : m_name(name) { }
	const std::string& getName() { return m_name; }

	virtual void apply(ofPixels& pixels) = 0;

private:
	std::string m_name;
};