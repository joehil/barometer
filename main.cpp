#include <Wire.h> 
#include <SPI.h>
#include <Adafruit_BMP085.h>
#include <RCSwitch.h>
#include "LowPower.h"

// Connect Vcc pin of the BMP085 to the Arduino pin 3.3V (5.0V NOT USE!) 
// Connect GND pin module to the Arduino GND 
// Connect the SCL pin module to analog pin 5 Arduino 
// Connect pin SDA module to pin 4 of analog Arduino 
// pin EOC (end of conversion) unused 
// XCLR pin is a reset pin is not used

Adafruit_BMP085 bmp;
RCSwitch mySwitch = RCSwitch();

#define RF_DATA 8
#define RF_VCC 7
#define RF_GND 9
#define LED 13
#define BAR_VCC 10
#define BAR_GND 11

signed int count = 0;
signed long temp = 0;
signed int press = 0;
signed long msg = 0;

void setup ()
{
  Serial.begin ( 9600 );

  // Transmitter is connected to Arduino Pin #10  
  mySwitch.enableTransmit(RF_DATA);
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(1);

  // Optional set pulse length.
  mySwitch.setPulseLength(320);
  
  // Optional set number of transmission repetitions.
  mySwitch.setRepeatTransmit(3);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(BAR_VCC, OUTPUT);
  pinMode(BAR_GND,OUTPUT);
  pinMode(RF_GND, OUTPUT);
  pinMode(RF_VCC, OUTPUT); 
  pinMode(RF_DATA, OUTPUT); 
  
  digitalWrite(RF_VCC, LOW);
  digitalWrite(RF_GND, LOW);
  digitalWrite(RF_DATA, LOW);
  digitalWrite(BAR_VCC, HIGH);
  digitalWrite(BAR_GND, HIGH);

  pinMode(12, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);

}

void loop ()
{
    digitalWrite(BAR_GND,LOW);
    delay(70);
    digitalWrite(LED,HIGH);
    delay(30);
    digitalWrite(LED,LOW);
    bmp.begin ();

    temp = bmp.readTemperature()*10;
    Serial.print ( "Temperature =" );
    Serial.print (temp);
    Serial.println ( "C *" );
    temp = temp * 10000;

    press = bmp.readPressure()/100;
    Serial.print ( "Pressure =" );
    Serial.print (press);
    Serial.println ( "hPa" );

    digitalWrite(BAR_GND, HIGH);

    msg = (signed long)-536870912 + temp + press;
    digitalWrite(RF_VCC, HIGH);
    mySwitch.send(msg,32); // 1879048192 legt das Device fest}
    digitalWrite(RF_VCC, LOW);

    count = 0;
    while (count < 80){
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
      count++;
    }

}
