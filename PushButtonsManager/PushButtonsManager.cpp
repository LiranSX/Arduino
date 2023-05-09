#include <Arduino.h>
#include <PushButtonsManager.h>

// BasePushButton

BasePushButton::BasePushButton(DebugWriter debugWriter, int pin)
{
	_pin = pin;
	_lastState = 0;
	_pressedInvocationRequired = true;
	_pressedStartTime = 0;
	Next = NULL;
	pinMode(pin, INPUT);
	Writer = debugWriter;
}

BasePushButton::~BasePushButton()
{}

void BasePushButton::Process(long currentTime)
{
	bool buttonState = IsPressed();

	if (buttonState) // pressed
	{
		if (_lastState) // "still" pushed?
		{
			if (_pressedInvocationRequired) // invocation required?
			{
				Press(currentTime - _pressedStartTime);
			}
		}
		else // "new" push
		{
			if (_pressedInvocationRequired)
			{
				_pressedInvocationRequired = Press(0);
			}
			_lastState = true;
			_pressedStartTime = currentTime;
		}
	}
	else // not pushed
	{
		if (_lastState) // was pushed before?
		{
			Release(currentTime - _pressedStartTime);
		}

		_lastState = false;
		_pressedInvocationRequired = true;
		_pressedStartTime = 0;
	}
}

// DigitalPushButton

DigitalPushButton::DigitalPushButton(DebugWriter debugWriter,
	int pin,
	bool pulledUp,
	DigitalPinPushButtonPressedEventHandler pressHandler,
	DigitalPinPushButtonReleasedEventHandler releaseHandler) : BasePushButton(debugWriter, pin)
{
	_pulledUp = pulledUp;
	_pressHandler = pressHandler;
	_releaseHandler = releaseHandler;
}

DigitalPushButton::~DigitalPushButton()
{
}

bool DigitalPushButton::IsPressed()
{
	bool isHigh = digitalRead(_pin);
	return (isHigh != _pulledUp);
}
bool DigitalPushButton::Press(long heldTime)
{
	DigitalPinPushButtonEventArgs eventArgs;
	
	eventArgs.Pin = _pin;
	eventArgs.HeldTime = heldTime;

	return _pressHandler(eventArgs);
}
void DigitalPushButton::Release(long heldTime)
{
	DigitalPinPushButtonEventArgs eventArgs;
	eventArgs.Pin = _pin;
	eventArgs.HeldTime = heldTime;
	_releaseHandler(eventArgs);
}


// AnalogPushButton

AnalogPushButton::AnalogPushButton(DebugWriter debugWriter,
	int pin,
	int id,
	int initialValue,
	int radius,
	AnalogPinPushButtonPressedEventHandler pressHandler,
	AnalogPinPushButtonReleasedEventHandler releaseHandler) : BasePushButton(debugWriter, pin)
{
	_id = id;
	_initialValue = initialValue;
	_radius = radius;
	_pressHandler = pressHandler;
	_releaseHandler = releaseHandler;
}

bool AnalogPushButton::IsPressed()
{
	int analogValue = analogRead(_pin);
	bool isPressed = (((_initialValue - _radius) < analogValue) && (analogValue < (_initialValue + _radius)));
	//Writer("_initialValue=%d, _radius=%d, analogValue=%d, isPressed=%d", _initialValue, _radius, analogValue, isPressed);
//	delay(50);
	return isPressed;
}
bool AnalogPushButton::Press(long heldTime)
{
	AnalogPinPushButtonEventArgs eventArgs;
	eventArgs.Id = _id;
	eventArgs.HeldTime = heldTime;

	return _pressHandler(eventArgs);
}
void AnalogPushButton::Release(long heldTime)
{
	AnalogPinPushButtonEventArgs eventArgs;
	eventArgs.Id = _id;
	eventArgs.HeldTime = heldTime;
	_releaseHandler(eventArgs);
}

AnalogPushButton::~AnalogPushButton()
{}


// PushButtonsManager

void PushButtonsManager::AddButton(BasePushButton* buttonDetails)
{
	BasePushButton* currentButton;
	if (_buttonsList == NULL)
	{
		currentButton = _buttonsList = buttonDetails;
	}
	else
	{
		currentButton = _buttonsList;
		while (currentButton->Next != NULL)
		{
			currentButton = currentButton->Next;
		}
		currentButton->Next = buttonDetails;
	}
}

void PushButtonsManager::RegisterDigitalPinButton(int pin, bool pulledUp, DigitalPinPushButtonPressedEventHandler pressedEventHandler, DigitalPinPushButtonReleasedEventHandler releasedEventHandler)
{
	DigitalPushButton* newButton = new DigitalPushButton(_debugWriter, pin, pulledUp, pressedEventHandler, releasedEventHandler);
	AddButton((BasePushButton*) newButton);
}

void PushButtonsManager::RegisterAnalogPinButton(int pin, int id, int initialValue, int radius, AnalogPinPushButtonPressedEventHandler pressedEventHandler, AnalogPinPushButtonReleasedEventHandler releasedEventHandler)
{
	AnalogPushButton* newButton = new AnalogPushButton(_debugWriter, pin, id, initialValue, radius, pressedEventHandler, releasedEventHandler);
	AddButton((BasePushButton*)newButton);
}

PushButtonsManager::PushButtonsManager(DebugWriter debugWriter)
{
	_buttonsList = NULL;
	_debugWriter = debugWriter;
}

PushButtonsManager::~PushButtonsManager()
{
	BasePushButton* next;

	while (_buttonsList != NULL)
	{
		next = _buttonsList->Next;
		delete _buttonsList;
		_buttonsList = next;
	}
}

void PushButtonsManager::Process(long currentTime)
{
	if (currentTime == 0)
	{
		currentTime = millis();
	}
	BasePushButton* test = _buttonsList;
	BasePushButton* currentButton = _buttonsList;

	while (currentButton != NULL)
	{
		currentButton->Process(currentTime);
		currentButton = currentButton->Next;
	}
	delay(10);
}

