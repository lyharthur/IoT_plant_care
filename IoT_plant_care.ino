#define BLYNK_PRINT Serial

#include <dht.h>
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <SimpleTimer.h>


char auth[] = "69ce88af6d0540728746cf4b9809357a";

#define DHTPIN A0     
#define redPin 3
#define greenPin 5
#define bluePin 6

dht DHT;
SimpleTimer timer;

int waterValue;
float t,w,h;

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
  range_1 = waterValue + 10;
  range_2 = waterValue - 10;

  if( w <= range_1 && w >= range_2){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 10);
    analogWrite(bluePin, 200);
  }
  else if( w > range_1 ){
    analogWrite(redPin, 255);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 10);
  }
  else if( w < range_2 ){
    analogWrite(redPin, 10);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 255);
  }
  
}

void checkWater(){
  Serial.print(" Slider Water Value is: ");
  Serial.println(waterValue);
}

BLYNK_WRITE(V3)
{
  waterValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  checkWater();
  plantMood();
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);
  BLYNK_WRITE(V3);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  // Setup a function to be called every second
  timer.setInterval(10000L, sendSensor);
  timer.setInterval(5000L, plantMood);
}

void loop()
{


  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
  
}
