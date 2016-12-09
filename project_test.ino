

#define BLYNK_PRINT Serial

#include <SPI.h>
#include <BridgeClient.h>
#include <BlynkSimpleYun.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "69ce88af6d0540728746cf4b9809357a";



WidgetLCD lcd(V1);

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  lcd.clear(); //Use it to clear the LCD Widget
  lcd.print(1, 0, "HI!!"); // use: (position X: 0-15, position Y: 0-1, "Message you want to print")
  lcd.print(1, 1, "It's time to bad");
  // Please use timed events when LCD printintg in void loop to avoid sending too many commands
  // It will cause a FLOOD Error, and connection will be dropped
}

BLYNK_WRITE(V2) {
  int x = param[0].asInt();
  int y = param[1].asInt();

  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  Serial.print("; Y = ");
  Serial.println(y);
}

void loop()
{
  Blynk.run();
}


