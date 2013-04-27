#include "CountDownElement.h"

CountDownElement::CountDownElement() : Element(500), m_isCountingDown(false), m_countDownDuration(0), m_countDownRunningTime(0)
{
	initialize();
}

CountDownElement::CountDownElement(const std::string& vertexShader, 
							const std::string& fragmentShader, 
							const std::string& geometryShader) :
	Element(500, vertexShader, fragmentShader, geometryShader), m_isCountingDown(false), m_countDownDuration(0), m_countDownRunningTime(0)
{
	initialize();
}

CountDownElement::~CountDownElement()
{
}

void CountDownElement::initialize()
{
	m_counter_background = ofImage("graphics/counter_background.png");
	m_counter_background.allocate(24, 430, OF_IMAGE_COLOR_ALPHA);
	
	m_counter_top_cap = ofImage("graphics/counter_top_cap.png");
	m_counter_top_cap.allocate(18, 3, OF_IMAGE_COLOR_ALPHA);

	m_counter_bottom_cap = ofImage("graphics/counter_bottom_cap.png");
	m_counter_bottom_cap.allocate(18, 3, OF_IMAGE_COLOR_ALPHA);

	m_counter_fill = ofImage("graphics/counter_fill.png");
	m_counter_fill.allocate(18, 1, OF_IMAGE_COLOR_ALPHA);

	m_counter_label = ofImage("graphics/counter_label.png");
	m_counter_label.allocate(84, 7, OF_IMAGE_COLOR_ALPHA);

	m_countDownTimerThresehold = 3;
	m_countDownColor = ofColor(2,166,79);
	m_countDownColorSecondary = ofColor(144,15,15);
	m_codeFont.loadFont("GUI/CODEBold.otf", 50);
}

void CountDownElement::startCountDown()
{
	m_isCountingDown = true;
	m_countDownRunningTime = 0;
}

void CountDownElement::setDuration(float duration)
{
	m_countDownDuration = duration;
}

void CountDownElement::update(float delta)
{
	Element::update(delta);

	if(!m_active || !m_isCountingDown)
	{
		return;
	}

	m_countDownRunningTime += delta;
}

void CountDownElement::preDraw()
{
	Element::preDraw();

	if(getShader())
	{
		//getShader()->setUniformTexture("tex", m_frameBuffer.getTextureReference(), 1);

		//getShader()->setUniform1f("progress_value", progress());
	}
}

void CountDownElement::onDraw()
{
	Element::onDraw();

	int timeLeftMS = (m_countDownDuration - m_countDownRunningTime);
	int timeLeft = ofClamp(timeLeftMS / 1000, 0, m_countDownDuration / 1000);

	float countdown_height_max = 420;
	
	float countdown_height = m_countDownDuration == 0 ? 0 : countdown_height_max * (timeLeftMS / m_countDownDuration);

	countdown_height = ofClamp(countdown_height, 0, countdown_height_max);
	
	if(countdown_height <= 0.0)
	{
		m_isCountingDown = false;
	}

	ofPushMatrix();

	m_counter_background.draw(115, 155);
	m_counter_label.draw(153, 512);

	ofTranslate(118, 155, 0);

	if(timeLeft <= m_countDownTimerThresehold) {
		ofSetColor(m_countDownColorSecondary);
	} else {
		ofSetColor(m_countDownColor);
	}

	m_counter_top_cap.draw(0, countdown_height_max - countdown_height);
	m_counter_fill.draw(0, countdown_height_max - countdown_height + 3, 18, countdown_height);
	m_counter_bottom_cap.draw(0, countdown_height_max + 3);

	ofTranslate(31, 426, 0);

	ostringstream convert;
	convert << timeLeft;

	m_codeFont.drawString(":", 0, 0);
	m_codeFont.drawString(convert.str(), 22, 0);
	

	ofSetColor(255,255,255);

	ofPopMatrix();
}