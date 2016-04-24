
// when button will be pressed, its pin number + how much time it was held will be sent. the return value indicates whether further calls are allowed:
// false = do not call for this button (for current press) any more
// true = keep calling with an increasing heldTime
typedef bool(*PushButtonPressedEvent)(int pin, long heldTime);

// will be called when a button is released along with the total pressed heldTime
typedef void(*PushButtonReleasedEvent)(int pin, long heldTime);

struct ButtonManagementDetails
{
	int Pin;
	PushButtonPressedEvent ButtonPressedHandler;
	PushButtonReleasedEvent ButtonReleasedHandler;

	bool LastState;
	bool PressedInvocationRequired;
	long PressedStartTime;
	ButtonManagementDetails* Next;
};

class PushButtonsManager
{
private:
	ButtonManagementDetails* _buttonsList;
	void AddButton(ButtonManagementDetails* buttonDetails);

public:
	PushButtonsManager();
	~PushButtonsManager();
	void RegisterButton(int pin, PushButtonPressedEvent pushButtonPressedEventHandler, PushButtonReleasedEvent pushButtonReleasedEventHandler);
	void Process(long currentTime = 0);
};








