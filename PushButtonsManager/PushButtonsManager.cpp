#include <Arduino.h>
#include <PushButtonsManager.h>

void PushButtonsManager::AddButton(ButtonManagementDetails* buttonDetails)
{
	ButtonManagementDetails* walker;
	if (_buttonsList == NULL)
	{
		walker = _buttonsList = buttonDetails;
	}
	else
	{
		walker = _buttonsList;
		while (walker->Next != NULL)
		{
			walker = walker->Next;
		}
		walker->Next = buttonDetails;
	}
}

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
		delete _buttonsList;
		_buttonsList = next;
	}
}

void PushButtonsManager::RegisterButton(int pin, PushButtonPressedEvent pushButtonPressedEventHandler, PushButtonReleasedEvent pushButtonReleasedEventHandler)
{
	ButtonManagementDetails* newButton = new ButtonManagementDetails();
	newButton->Pin = pin;
	newButton->ButtonPressedHandler = pushButtonPressedEventHandler;
	newButton->ButtonReleasedHandler = pushButtonReleasedEventHandler;
	newButton->LastState = 0;
	newButton->PressedInvocationRequired = true;
	newButton->PressedStartTime = 0;
	newButton->Next = NULL;
	AddButton(newButton);
	pinMode(newButton->Pin, INPUT);
}


void PushButtonsManager::Process(long currentTime)
{
	if (currentTime == 0)
	{
		currentTime = millis();
	}

	ButtonManagementDetails* walker = _buttonsList;
	while (walker != NULL)
	{
		bool buttonState = digitalRead(walker->Pin);
		if (buttonState) // high
		{
			if (walker->LastState) // "still" high?
			{
				if (walker->PressedInvocationRequired) // invocation required?
				{
					if (walker->PressedInvocationRequired != NULL)
					{
						walker->PressedInvocationRequired = walker->ButtonPressedHandler(walker->Pin, currentTime - walker->PressedStartTime);
					}
				}
			}
			else // "new" high
			{
				if (walker->PressedInvocationRequired != NULL)
				{
					walker->PressedInvocationRequired = walker->ButtonPressedHandler(walker->Pin, 0);
				}
				walker->LastState = true;
				walker->PressedStartTime = currentTime;
			}
		}
		else // low
		{
			if (walker->LastState) // was high before?
			{
				if (walker->ButtonReleasedHandler != NULL)
				{
					walker->ButtonReleasedHandler(walker->Pin, currentTime - walker->PressedStartTime);
				}
			}
			walker->LastState = false;
			walker->PressedInvocationRequired = true;
			walker->PressedStartTime = 0;
		}

		walker = walker->Next;
	}
}
