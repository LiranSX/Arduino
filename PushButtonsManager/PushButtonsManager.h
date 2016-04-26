

struct DigitalPinPushButtonEventArgs
{
	int Pin;
	long HeldTime;
};

struct AnalogPinPushButtonEventArgs
{
	char* Label;
	long HeldTime;
};

// when button will be pressed, its identifier (name for analog, pin for digital) + how much time it was held will be sent. the return value indicates whether further calls are allowed:
// false = do not call for this button (for current press) any more
// true = keep calling with an increasing heldTime
typedef bool(*DigitalPinPushButtonPressedEvent)(DigitalPinPushButtonEventArgs e);
typedef bool(*AnalogPinPushButtonPressedEvent)(AnalogPinPushButtonEventArgs e);

// will be called when a button is released along with the total pressed heldTime
typedef void(*DigitalPinPushButtonReleasedEvent)(DigitalPinPushButtonEventArgs e);
typedef void(*AnalogPinPushButtonReleasedEvent)(AnalogPinPushButtonEventArgs e);

typedef enum PinType { Digital, Analog };


struct ButtonManagementDetails
{
	PinType Type;
	int Pin;
	bool LastState;
	bool PressedInvocationRequired;
	long PressedStartTime;
	ButtonManagementDetails* Next;
};

struct DigitalPinButtonManagementDetails : ButtonManagementDetails
{
	DigitalPinButtonManagementDetails()
	{
		Type = Digital;
	}

	bool PulledUp;
	DigitalPinPushButtonPressedEvent PressedHandler;
	DigitalPinPushButtonReleasedEvent ReleasedHandler;
};

struct AnalogPinButtonManagementDetails : ButtonManagementDetails
{
	AnalogPinButtonManagementDetails()
	{
		Type = Analog;
	}

	char* Label;
	int InitialValue;
	int Radius;
	AnalogPinPushButtonPressedEvent PressedHandler;
	AnalogPinPushButtonReleasedEvent ReleasedHandler;
};


class PushButtonsManager
{
private:
	ButtonManagementDetails* _buttonsList;
	void AddButton(ButtonManagementDetails* buttonDetails);
	bool IsPushed(ButtonManagementDetails* buttonDetails);
	void SubmitButton(ButtonManagementDetails* buttonDetails, int pin);
	void InvokePressHandler(ButtonManagementDetails* currentButton, long heldTime);
	void InvokeReleaseHandler(ButtonManagementDetails* currentButton, long heldTime);

public:
	PushButtonsManager();
	~PushButtonsManager();
	void RegisterDigitalPinButton(int pin, bool pulledUp, DigitalPinPushButtonPressedEvent pressedEventHandler, DigitalPinPushButtonReleasedEvent releasedEventHandler);
	void RegisterAnalogPinButton(int pin, char* label, int initialValue, int radius, AnalogPinPushButtonPressedEvent pressedEventHandler, AnalogPinPushButtonReleasedEvent releasedEventHandler);
	void Process(long currentTime = 0);
};








