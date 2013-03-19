#ifndef BOOF_TIMER
#define BOOF_TIMER

#include <vector>

class Timer {
protected:
	static unsigned long long m_startTime;
	static bool stopped;
	static bool smooth;
	static int sampleIndex;
	static const int sampleSize = 11;
	static const int noiseConstant = 4;
	static float timeScale;
	static std::vector<double> history;
public:
	static void Initialize(bool start, bool smooth);

	static void Stop();
	static void Start();
	static double TimerUpdate();
};

#endif