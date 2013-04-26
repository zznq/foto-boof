#include "Timer.h"
#include <algorithm>

//Forward Decleration
unsigned long long ofGetElapsedTimeMillis();

unsigned long long Timer::m_startTime = 0;
bool Timer::stopped = false;
int Timer::sampleIndex = 0;
float Timer::timeScale = 1.0f;
bool Timer::smooth = false;
std::vector<double> Timer::history = std::vector<double>(sampleSize, 0);

void Timer::Initialize(bool start, bool smooth) {
	m_startTime = ofGetElapsedTimeMillis();
	Timer::stopped = !start;
	Timer::smooth = smooth;
}

void Timer::Stop() {
	Timer::stopped = true;	
}

void Timer::Start() {
	Timer::stopped = false;	
}

double Timer::TimerUpdate() {
	static unsigned long long m_lastElapsedTime = 0;
	unsigned long long of_elapsedTime = ofGetElapsedTimeMillis() - m_startTime;

	long delta;

	delta = (of_elapsedTime - m_lastElapsedTime);
	
	if(Timer::stopped) {
		return 0.0;
	}

	m_lastElapsedTime = of_elapsedTime;

	
	double elapsed_time = Timer::timeScale * delta;
	
	if(!Timer::smooth)
	{
		return elapsed_time;
	}
	else
	{
		Timer::history[Timer::sampleIndex] = elapsed_time;
		Timer::sampleIndex = (Timer::sampleIndex + 1) % Timer::sampleSize;

		double sum = 0;

		sort(Timer::history.begin(), Timer::history.end());

		for (int i=0; i<Timer::history.size(); i++) {
			//Discard top two and bottom two results
			if(i >= (noiseConstant /2) && i < (Timer::history.size() - noiseConstant /2)){
				sum += Timer::history.at(i); 
			}
		}

		return sum / ((double)Timer::history.size() - noiseConstant);
	}
}