
class VibrationSensor
{
protected:
	int _pinNumber;

public:
	VibrationSensor(int pinNumber);
	bool IsVibrating();
	long GetVibrationStrength();
};

