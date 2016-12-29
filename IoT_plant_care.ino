#define BLYNK_PRINT Serial

#include <dht.h>
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <SimpleTimer.h>
#include <Servo.h>


char auth[] = "69ce88af6d0540728746cf4b9809357a";

#define DHTPIN A0     
#define redPin 3
#define greenPin 5
#define bluePin 6


WidgetLCD lcd(V1);
dht DHT;
SimpleTimer timer;
Servo myservo;  

int waterValue;
float t,w,h;
int pos = 0; 

void sendSensor()
{
  h = DHT.humidity;
  t = DHT.temperature; 
  w = analogRead(A1)/10;
  DHT.read11(DHTPIN);
  Serial.print("Current humidity = "); 
  Serial.print(h); 
  Serial.print("% "); 
  Serial.print("temperature = ");
  Serial.print(t); 
  Serial.print("C ");
  Serial.print("Moisture Sensor Value: ");
  Serial.println(w);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V4, w);
  
}

void plantMood(){
  int range_1,range_2;
  range_1 = waterValue + 7;
  range_2 = waterValue - 7;
  
  lcd.print(0, 0, "Dear Arthur,  ");
  if( w <= range_1 && w >= range_2){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 10);
    analogWrite(bluePin, 200);
    lcd.print(0, 1, "I feel Good ~~~~");
  }
  else if( w > range_1 ){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 10);
    lcd.print(0, 1, "I'm swimming.....");
  }
  else if( w < range_2 ){
    analogWrite(redPin, 10);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
    lcd.print(0, 1, "I'm thirsty QQ   ");
  }
}

void checkWater(){
  
  w = analogRead(A1)/10;
  if(w < waterValue - 10){// need water . Open
    myservo.write(1);     
  }
   if(w >= waterValue){// don't need water. Close

    myservo.write(179);     
  }
  
}

BLYNK_WRITE(V3)
{
  waterValue = param.asInt(); 
  Serial.print(" Slider Water Value is: ");
  Serial.println(waterValue);
  plantMood();
  checkWater();
}

void setup()
{
  myservo.attach(9); 
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);
  BLYNK_WRITE(V3);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  lcd.clear();
  
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(500L, plantMood);
  timer.setInterval(500L, checkWater);
  
}

void loop()
{
  
  Blynk.run(); // Initiates Blynk
  
  timer.run(); // Initiates SimpleTimer
}
