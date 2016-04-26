#include <PushButtonsManager.h>

PushButtonsManager buttonsManager;

#define LEFT_BUTTON_PIN 2
#define RIGHT_BUTTON_PIN 4
#define ANALOG_BUTTONS_PIN A7

void setup() {
  Serial.begin(115200);

  buttonsManager.RegisterDigitalPinButton(RIGHT_BUTTON_PIN, false, digitalPressedHandler, digitalReleasedHandler);
  buttonsManager.RegisterDigitalPinButton(LEFT_BUTTON_PIN, false, digitalPressedHandler, digitalReleasedHandler);

  buttonsManager.RegisterAnalogPinButton(ANALOG_BUTTONS_PIN, "Right", 730, 30, analogPressedHandler, analogReleasedHandler);
  buttonsManager.RegisterAnalogPinButton(ANALOG_BUTTONS_PIN, "Left", 790, 30, analogPressedHandler, analogReleasedHandler);

  Serial.println("ready");
}

bool analogPressedHandler(AnalogPinPushButtonEventArgs e)
{
  bool result = true;
  
  Serial.print("analog pressed: ");
  Serial.print(e.Label);
  Serial.print(", held: ");
  Serial.println(e.HeldTime);
  delay(500);
  return result;
}

void analogReleasedHandler(AnalogPinPushButtonEventArgs e)
{
  Serial.print("analog released: ");
  Serial.print(e.Label);
  Serial.print(", held: ");
  Serial.println(e.HeldTime);
  delay(500);
}

bool digitalPressedHandler(DigitalPinPushButtonEventArgs e)
{
  bool result = true;
  
  Serial.print("digital pressed: ");
  Serial.print(e.Pin);
  Serial.print(", held: ");
  Serial.println(e.HeldTime);
 
  delay(500);
  return result;
}

void digitalReleasedHandler(DigitalPinPushButtonEventArgs e)
{
  Serial.print("digital released: ");
  Serial.print(e.Pin);
  Serial.print(", held: ");
  Serial.println(e.HeldTime);

  delay(500);
}

void loop()
{
  buttonsManager.Process();
}
