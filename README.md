# Arduino
My Aduino Libraries

PushButtonsManager
------------------
  allows to use push buttons and process their events easily.
* add event handler for "pushed" event:
    will receive the pin number of the pushed button + how much time it was held since the first "high" value.
    return "true" to keep getting callback calls for the button's current held session or "false" to stop receiving them (for current push).
* add event handler for "released" event:
    will receive the pin number of the pushed button + how much time it was held since the first "high" value.

