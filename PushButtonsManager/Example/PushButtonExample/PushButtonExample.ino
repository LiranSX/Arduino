#include <PushButtonsManager.h>

// pin definitions

#define PIN_BUTTON1    15
#define PIN_BUTTON2    4
#define PIN_BUTTONA    A0

#define BUTTON_ID_3    4
#define BUTTON_ID_4    5

#define MAX_SERIAL_OUTPUT_BUFFER    1024
char debugOutputBuffer[MAX_SERIAL_OUTPUT_BUFFER];

PushButtonsManager buttonsManager(publishDebug);

void setup()
{
    Serial.begin(115200);
    buttonsManager.RegisterDigitalPinButton(PIN_BUTTON1, false, digitalButtonPressedHandler, digitalButtonReleasedHandler);
    buttonsManager.RegisterDigitalPinButton(PIN_BUTTON2, false, digitalButtonPressedHandler, digitalButtonReleasedHandler);

    buttonsManager.RegisterAnalogPinButton(PIN_BUTTONA, BUTTON_ID_3, 1024, 150, analogButtonPressedHandler, analogButtonReleasedHandler);
    buttonsManager.RegisterAnalogPinButton(PIN_BUTTONA, BUTTON_ID_4, 602, 150, analogButtonPressedHandler, analogButtonReleasedHandler);
}


void loop()
{
    long now = millis();
    buttonsManager.Process();
}

void publishDebug(const char* __fmt, ...)
{
    va_list args;
    va_start(args, __fmt);
    vsprintf(debugOutputBuffer, __fmt, args);
    va_end(args);
    Serial.println(debugOutputBuffer);
}

bool digitalButtonPressedHandler(DigitalPinPushButtonEventArgs e)
{
    bool contigousPress = false;

    switch (e.Pin)
    {
    case PIN_BUTTON1:
        Serial.println("digital button 1 pressed");
        break;
    case PIN_BUTTON2:
        Serial.println("digital button 2 pressed");
        break;
    }

    return contigousPress;
}

void digitalButtonReleasedHandler(DigitalPinPushButtonEventArgs e)
{
    if (e.HeldTime > 1000 && e.Pin == PIN_BUTTON1)
    {
        Serial.println("digital button 1 released after 1000ms");
    }
}

bool analogButtonPressedHandler(AnalogPinPushButtonEventArgs e)
{
    switch (e.Id)
    {
    case BUTTON_ID_3:
        Serial.println("analog button 3 pressed");
        break;
    case BUTTON_ID_4:
        Serial.println("analog button 4 pressed");
        break;
    }
    return false;
}
void analogButtonReleasedHandler(AnalogPinPushButtonEventArgs e)
{
    Serial.println("analog button released");
}
