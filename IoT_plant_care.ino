

#define BLYNK_PRINT Serial

#include <dht.h>
#include <BlynkSimpleYun.h>
#include <SimpleTimer.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "69ce88af6d0540728746cf4b9809357a";


#define DHTPIN A0          // What digital pin we're connected to


dht DHT;
SimpleTimer timer;


void sendSensor()
{
  float h = DHT.humidity;
  float t = DHT.temperature; 
  DHT.read11(DHTPIN);
  Serial.print("Current humidity = "); 
  Serial.print(h); 
  Serial.print("% "); 
  Serial.print("temperature = ");
  Serial.print(t); 
  Serial.println("C ");

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);



  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
  
}



