// Arduino Script for WIFI LED Strip Controller

#include <SoftwareSerial.h>
#include <EEPROM.h>

#define RX 11
#define TX 10

#define GREEN 9
#define RED 6
#define BLUE 5

int r = 0;
int g = 0;
int b = 0;


SoftwareSerial softSerial(RX, TX);

void setup() {
  Serial.begin(9600);
  softSerial.begin(9600);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.println("ARDUINO say HELLO!");

  bootLED(); 
}

void loop() {
  if (softSerial.available()) {
    String data = softSerial.readString();
    Serial.print("SoftSerial said >>");
    Serial.println(data);

    if(String command = data.substring(0, data.indexOf(":"))) {
//    Serial.print("ARDUINO HANDLE COMMAND: ");
//    Serial.println(command);
  
      if (command == "SETCOLOR") {
        r = data.substring(data.indexOf("r=") + 2, data.indexOf(",g=")).toInt();
        g = data.substring(data.indexOf("g=") + 2, data.indexOf(",b=")).toInt();
        b = data.substring(data.indexOf("b=") + 2, data.indexOf("\r\n")).toInt();
  
        changeColor();
      }
//      if (command == "GETCOLOR") {
//        softSerial.print("COLORIS:");
//        softSerial.print("r=");
//        softSerial.print(r);
//        softSerial.print(",g=");
//        softSerial.print(g);
//        softSerial.print(",b=");
//        softSerial.println(b);
//       }
    }
    
    if (Serial.available()) {
      softSerial.write(Serial.read());
    }
  }
}

void bootLED() {
  r = EEPROM.read(0);
  g = EEPROM.read(1);
  b = EEPROM.read(2);
  
  changeColor();
  
  softSerial.print("COLORIS:");
  softSerial.print("r=");
  softSerial.print(r);
  softSerial.print(",g=");
  softSerial.print(g);
  softSerial.print(",b=");
  softSerial.println(b);
}

void changeColor() {
  if(r < 1) {
    r = 0;
    digitalWrite(RED, LOW);
  } else {
    if(r > 254) {
      digitalWrite(RED, HIGH);
    } else {
      analogWrite(RED, r);
    }
  }
  
  if(g < 1) {
    g = 0;
    digitalWrite(GREEN, LOW);
  } else {
    if(g > 254) {
      digitalWrite(GREEN, HIGH);
    } else {
      analogWrite(GREEN, g);
    }
  }
  
  if(b < 1) {
    b = 0;
    digitalWrite(BLUE, LOW);
  } else {
    if(b > 254) {
      digitalWrite(BLUE, HIGH);
    } else {
      analogWrite(BLUE, b);
    }
  }
    
  EEPROM.update(0, r);
  EEPROM.update(1, g);
  EEPROM.update(2, b);
}
