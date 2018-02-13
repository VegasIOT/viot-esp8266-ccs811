/***************************************************************************
  This is a library for the CCS811 air 

  This sketch reads the sensor

  Designed specifically to work with the Adafruit CCS811 breakout
  ----> http://www.adafruit.com/products/3566

  These sensors use I2C to communicate. The device's I2C address is 0x5A

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_CCS811.h"
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"'

Adafruit_CCS811 ccs;
// Initialize the OLED display using Wire library
SSD1306  display(0x3c, D5, D6);
//D4 reset

void setup() {
  Serial.begin(115200);
  
  Serial.println("CCS811 test");
  
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);  //10 16 24
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(1, 1, 128,"Startup");
  display.display();
  delay(5000);
  
 
  //Wire.begin(D5,D6); //sda scl
  
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
      digitalWrite(D4,LOW);
      delay(100);
      digitalWrite(D4,HIGH);
      software_Reset();
  }

  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  
  ccs.setTempOffset(temp - 25.0);
}

void loop() {
  if(ccs.available()){
    float temp = ccs.calculateTemperature();
    float fahrenheit;
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.print("ppb   Temp:");
      fahrenheit = ((temp * 9)/5) + 32;
      Serial.println(fahrenheit);
      display.clear();
      display.drawStringMaxWidth(1, 1, 128,"T:"+String(int(fahrenheit))+"_V:"+String(ccs.getTVOC())+" C:"+String(ccs.geteCO2()));
      display.display();
      /*
       
      delay(1000);
      display.clear();
      display.drawStringMaxWidth(1, 1, 128,"CO2:"+String(ccs.geteCO2())+" TVOC:"+String(ccs.getTVOC()));
      display.display();
      delay(1000);
      */
    }
    else{
      Serial.println("ERROR!");
      digitalWrite(D4,LOW);
      delay(100);
      digitalWrite(D4,HIGH);
      software_Reset();
      
      //while(1);
    }
  }
  delay(100);
}

/****reset***/
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
Serial.print("resetting");
ESP.reset(); 
}
