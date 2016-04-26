#include <Arduino.h>
#include <PushButtonsManager.h>


// private
bool PushButtonsManager::IsPushed(ButtonManagementDetails* buttonDetails)
{
	bool isPushed = false;

	if (buttonDetails->Type == Digital)
	{
		DigitalPinButtonManagementDetails* digital = (DigitalPinButtonManagementDetails*)buttonDetails;
		bool isHigh = digitalRead(digital->Pin);
		isPushed = isHigh ^ digital->PulledUp; // xor
	}
	else if (buttonDetails->Type == Analog)
	{
		AnalogPinButtonManagementDetails* analog = (AnalogPinButtonManagementDetails*)buttonDetails;
		int analogValue = analogRead(analog->Pin);
		isPushed = (analog->InitialValue - analog->Radius) < analogValue &&  analogValue < (analog->InitialValue + analog->Radius);
	}

	return isPushed;
}

void PushButtonsManager::SubmitButton(ButtonManagementDetails* buttonDetails, int pin)
{
	buttonDetails->LastState = 0;
	buttonDetails->PressedInvocationRequired = true;
	buttonDetails->PressedStartTime = 0;
	buttonDetails->Next = NULL;
	buttonDetails->Pin = pin;
	pinMode(pin, INPUT);
	AddButton(buttonDetails);
}

void PushButtonsManager::AddButton(ButtonManagementDetails* buttonDetails)
{
	ButtonManagementDetails* currentButton;
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

void PushButtonsManager::InvokePressHandler(ButtonManagementDetails* currentButton, long heldTime)
{
	if (currentButton->PressedInvocationRequired != NULL)
	{
		if (currentButton->Type == Digital)
		{
			DigitalPinButtonManagementDetails* digitalButton = (DigitalPinButtonManagementDetails*)currentButton;

			DigitalPinPushButtonEventArgs e;
			e.Pin = digitalButton->Pin;
			e.HeldTime = heldTime;
			digitalButton->PressedInvocationRequired = digitalButton->PressedHandler(e);
		}
		else if (currentButton->Type == Analog)
		{
			AnalogPinButtonManagementDetails* analogButton = (AnalogPinButtonManagementDetails*)currentButton;

			AnalogPinPushButtonEventArgs e;
			e.Label = new char[strlen(analogButton->Label) + 1];
			strcpy(e.Label, analogButton->Label);
			e.HeldTime = heldTime;
			analogButton->PressedInvocationRequired = analogButton->PressedHandler(e);
			delete e.Label;
		}
	}
}

void PushButtonsManager::InvokeReleaseHandler(ButtonManagementDetails* currentButton, long heldTime)
{
	if (currentButton->Type == Digital)
	{
		DigitalPinButtonManagementDetails* digitalButton = (DigitalPinButtonManagementDetails*)currentButton;
		if (digitalButton->ReleasedHandler != NULL)
		{
			DigitalPinPushButtonEventArgs e;
			e.Pin = digitalButton->Pin;
			e.HeldTime = heldTime;
			digitalButton->ReleasedHandler(e);
		}
	}
	else if (currentButton->Type == Analog)
	{
		AnalogPinButtonManagementDetails* analogButton = (AnalogPinButtonManagementDetails*)currentButton;

		if (analogButton->ReleasedHandler != NULL)
		{
			AnalogPinPushButtonEventArgs e;
			e.Label = new char[strlen(analogButton->Label) + 1];
			strcpy(e.Label, analogButton->Label);
			e.HeldTime = heldTime;
			analogButton->ReleasedHandler(e);
			delete e.Label;
		}
	}
}


// public
PushButtonsManager::PushButtonsManager()
{
	_buttonsList = NULL;
}

PushButtonsManager::~PushButtonsManager()
{
	ButtonManagementDetails* next;

	while (_buttonsList != NULL)
	{
		next = _buttonsList->Next;
		if (_buttonsList->Type == Analog)
		{
			delete(((AnalogPinButtonManagementDetails*)_buttonsList)->Label);
		}
		delete _buttonsList;
		_buttonsList = next;
	}
}

void PushButtonsManager::RegisterDigitalPinButton(int pin, bool pulledUp, DigitalPinPushButtonPressedEvent pressedEventHandler, DigitalPinPushButtonReleasedEvent releasedEventHandler)
{
	DigitalPinButtonManagementDetails* newButton = new DigitalPinButtonManagementDetails();

	newButton->PulledUp = pulledUp;
	newButton->PressedHandler = pressedEventHandler;
	newButton->ReleasedHandler = releasedEventHandler;

	SubmitButton(newButton, pin);
}

void PushButtonsManager::RegisterAnalogPinButton(int pin, char* label, int initialValue, int radius, AnalogPinPushButtonPressedEvent pressedEventHandler, AnalogPinPushButtonReleasedEvent releasedEventHandler)
{
	AnalogPinButtonManagementDetails* newButton = new AnalogPinButtonManagementDetails();
	
	newButton->Label = new char[strlen(label) + 1];
	strcpy(newButton->Label, label);
	newButton->InitialValue = initialValue;
	newButton->Radius = radius;

	newButton->PressedHandler = pressedEventHandler;
	newButton->ReleasedHandler = releasedEventHandler;

	SubmitButton(newButton, pin);
}



void PushButtonsManager::Process(long currentTime)
{
	if (currentTime == 0)
	{
		currentTime = millis();
	}

	ButtonManagementDetails* currentButton = _buttonsList;
	while (currentButton != NULL)
	{
		bool buttonState = IsPushed(currentButton);

		if (buttonState) // pushed
		{
			if (currentButton->LastState) // "still" pushed?
			{
				if (currentButton->PressedInvocationRequired) // invocation required?
				{
					if (currentButton->PressedInvocationRequired != NULL)
					{
						InvokePressHandler(currentButton, currentTime - currentButton->PressedStartTime);
					}
				}
			}
			else // "new" push
			{
				if (currentButton->PressedInvocationRequired != NULL)
				{
					InvokePressHandler(currentButton, 0);
				}
				currentButton->LastState = true;
				currentButton->PressedStartTime = currentTime;
			}
		}
		else // not pushed
		{
			if (currentButton->LastState) // was pushed before?
			{
				InvokeReleaseHandler(currentButton, currentTime - currentButton->PressedStartTime);
			}
			currentButton->LastState = false;
			currentButton->PressedInvocationRequired = true;
			currentButton->PressedStartTime = 0;
		}

		currentButton = currentButton->Next;
	}
}
