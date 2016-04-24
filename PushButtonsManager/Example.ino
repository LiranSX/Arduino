#include <PushButtonsManager.h>

PushButtonsManager buttonsManager;

#define LEFT_BUTTON 4
#define RIGHT_BUTTON 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  buttonsManager.RegisterButton(RIGHT_BUTTON, pressedHandler, releasedHandler);
  buttonsManager.RegisterButton(LEFT_BUTTON, pressedHandler, releasedHandler);

  Serial.println("ready");
}

bool pressedHandler(int pin, long heldTime)
{
  bool result = true;
  
  Serial.print("pressed: ");
  Serial.print(pin);
  Serial.print(", held: ");
  Serial.println(heldTime);
 
  return result;
}

void releasedHandler(int pin, long totalHeldTime)
{
  Serial.print("released: ");
  Serial.print(pin);
  Serial.print(", held: ");
  Serial.println(totalHeldTime);
}

void loop()
{
  buttonsManager.Process();
 
}
