
#include <VibrationSensor.h>

class VibrationSensorAlarm : VibrationSensor
{
private:
	// from user
	int _vibrationsTimeFrameMeasure;
	int _bustedVibrationsCount;
	int _minReTriggerTime;

	// for local use
	long _vibrationStartTime;
	int _vibrationsCount;
	long _lastBustedTime;

public:
	VibrationSensorAlarm(int pinNumber, int bustedVibrationsCount, int vibrationsTimeFrameMeasureSecs, int minReTriggerTimeSecs);

	bool isBusted(long currentTime);
};

