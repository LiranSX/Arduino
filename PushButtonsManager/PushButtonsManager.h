#ifndef PUSH_BUTTON_MANAGER
#define PUSH_BUTTON_MANAGER

typedef void (*DebugWriter)(const char* __fmt, ...);

struct DigitalPinPushButtonEventArgs
{
	int Pin;
	long HeldTime;
};

struct AnalogPinPushButtonEventArgs
{
	int Id;
	long HeldTime;
};

// when button will be pressed, its identifier (id for analog, pin# for digital) + how much time it was held will be sent. the return value indicates whether further calls are allowed:
// false = do not call for this button (for current press) any more
// true = keep calling with an increasing heldTime
typedef bool(*DigitalPinPushButtonPressedEventHandler)(DigitalPinPushButtonEventArgs e);
typedef bool(*AnalogPinPushButtonPressedEventHandler)(AnalogPinPushButtonEventArgs e);

// will be called when a button is released along with the total pressed heldTime
typedef void(*DigitalPinPushButtonReleasedEventHandler)(DigitalPinPushButtonEventArgs e);
typedef void(*AnalogPinPushButtonReleasedEventHandler)(AnalogPinPushButtonEventArgs e);

class BasePushButton
{
private:
	bool _lastState;
	bool _pressedInvocationRequired;
	long _pressedStartTime;

protected:
	int _pin;
protected:
	DebugWriter Writer;

public:
	BasePushButton(DebugWriter debugWriter, int pin);
	~BasePushButton();
	void Process(long currentTime);
	virtual bool IsPressed() = 0;
	virtual bool Press(long heldTime) = 0;
	virtual void Release(long heldTime) = 0;
	BasePushButton* Next;
};

class DigitalPushButton : BasePushButton
{
private:
	bool _pulledUp;
	DigitalPinPushButtonPressedEventHandler _pressHandler;
	DigitalPinPushButtonReleasedEventHandler _releaseHandler;

public:
	DigitalPushButton(DebugWriter debugWriter,
		int pin,
		bool pulledUp,
		DigitalPinPushButtonPressedEventHandler pressHandler,
		DigitalPinPushButtonReleasedEventHandler releaseHandler);
	bool IsPressed();
	bool Press(long heldTime);
	void Release(long heldTime);
	~DigitalPushButton();
};

class AnalogPushButton : BasePushButton
{
private:
	int _initialValue;
	int _radius;
	int _id;

	AnalogPinPushButtonPressedEventHandler _pressHandler;
	AnalogPinPushButtonReleasedEventHandler _releaseHandler;
public:
	AnalogPushButton(DebugWriter debugWriter,
		int pin,
		int id,
		int initialValue,
		int radius,
		AnalogPinPushButtonPressedEventHandler pressHandler,
		AnalogPinPushButtonReleasedEventHandler releaseHandler);
	bool IsPressed();
	bool Press(long heldTime);
	void Release(long heldTime);
	~AnalogPushButton();
};

class PushButtonsManager
{
private:
	BasePushButton* _buttonsList;
	void AddButton(BasePushButton* buttonDetails);
	DebugWriter _debugWriter;
public:
	PushButtonsManager(DebugWriter debugWriter);
	~PushButtonsManager();
	void RegisterDigitalPinButton(int pin, bool pulledUp, DigitalPinPushButtonPressedEventHandler pressedEventHandler, DigitalPinPushButtonReleasedEventHandler releasedEventHandler);
	void RegisterAnalogPinButton(int pin, int id, int initialValue, int radius, AnalogPinPushButtonPressedEventHandler pressedEventHandler, AnalogPinPushButtonReleasedEventHandler releasedEventHandler);
	void Process(long currentTime = 0);
};

#endif

