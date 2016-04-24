
#include "Arduino.h"
#include <VibrationSensor.h>

VibrationSensor::VibrationSensor(int pinNumber)
{
	_pinNumber = pinNumber;
	pinMode(_pinNumber, INPUT);
}

bool VibrationSensor::IsVibrating()
{
	bool result = digitalRead(_pinNumber);
	return result;
}

long VibrationSensor::GetVibrationStrength()
{
	delay(10);
	unsigned long measurement = pulseIn(_pinNumber, HIGH);
	return measurement;
}


