/*
  AirQuality Demo V1.0.
  connect to A0 to start testing. it will needs about 20s to start 
* By: http://www.seeedstudio.com
*/
#include"AirQuality.h"
#include"Arduino.h"
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11 

AirQuality airqualitysensor;
int current_quality =-1;

boolean increased = false;
float last = 1;
boolean initialize = false;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
    Serial.begin(9600);
    airqualitysensor.init(14);
    pinMode(52, OUTPUT);
    dht.begin();
}
void loop()
{
    current_quality=airqualitysensor.slope();
//    if (current_quality >= 0)// if a valid data returned.
//    {
//        if (current_quality==0)
//            Serial.println("High pollution! Force signal active");
//        else if (current_quality==1)
//            Serial.println("High pollution!");
//        else if (current_quality==2)
//            Serial.println("Low pollution!");
//        else if (current_quality ==3)
//            Serial.println("Fresh air");
//    }


    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if(initialize == false)
    {
      initialize = true;
      last = t;
    }
    else
    {
      if(t > last)
      {
        increased = true;
        last = t;
      }
      else if(t < last)
      {
        increased = false;
        last = t;
      }
    }
    digitalWrite(52, increased);
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        Serial.print("Humidity: "); 
        Serial.print(h);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(t);
        Serial.println(" *C");
    }
    delay(1500);
}
ISR(TIMER1_OVF_vect)
{
  if(airqualitysensor.counter==61)//set 2 seconds as a detected duty
  {

      airqualitysensor.last_vol=airqualitysensor.first_vol;
      airqualitysensor.first_vol=analogRead(A0);
      airqualitysensor.counter=0;
      airqualitysensor.timer_index=1;
      PORTB=PORTB^0x20;
  }
  else
  {
    airqualitysensor.counter++;
  }
} 
