
#include "Arduino.h"
#include <VibrationSensorAlarm.h>


VibrationSensorAlarm::VibrationSensorAlarm(int pinNumber, int bustedVibrationsCount, int vibrationsTimeFrameMeasureSecs, int minReTriggerTimeSecs)
	: VibrationSensor(pinNumber)
{
	_bustedVibrationsCount = bustedVibrationsCount;
	_vibrationsTimeFrameMeasure = vibrationsTimeFrameMeasureSecs * 1000;
	_minReTriggerTime = minReTriggerTimeSecs * 1000;
	_vibrationStartTime = _vibrationsCount = _lastBustedTime = 0;
}

bool VibrationSensorAlarm::isBusted(long currentTime = 0)
{
	bool result = false;

	if (currentTime == 0)
	{
		currentTime = millis();
	}

	if (_vibrationStartTime != 0 && (currentTime - _vibrationStartTime > _vibrationsTimeFrameMeasure))
	{
		_vibrationStartTime = 0;
		_vibrationsCount = 0;
	}

	if (IsVibrating())
	{
		_vibrationsCount++;
#ifdef DEBUG
		Serial.print("vibrations: ");
		Serial.println(vibrationCount);
#endif
		if (_vibrationStartTime == 0)
		{
			_vibrationStartTime = currentTime;
		}
		if (_vibrationsCount >= _bustedVibrationsCount && currentTime - _vibrationStartTime < _vibrationsTimeFrameMeasure)
		{
			_vibrationsCount = 0;
			if (currentTime - _lastBustedTime > _minReTriggerTime)
			{
				_lastBustedTime = currentTime;
				result = true;
			}
		}
	}
	return result;
}
