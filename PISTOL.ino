#include <TM1637Display.h>
#include <Ultrasonic.h>
Ultrasonic ultrasonic (5, 6);

const int buttonPin = 2;
const int laserPin = 7;

int distance;

#define CLK 4  
#define DIO 3  

TM1637Display display(CLK, DIO);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(laserPin, OUTPUT);

  digitalWrite(laserPin, LOW); 

  Serial.begin(9600);

  display.setBrightness(0x0F);  
  display.clear();              
}

void loop() {


  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { 
    Serial.print("Distance in CM: ");
    Serial.println(ultrasonic.distanceRead());
    delay(50);
    display.showNumberDec(ultrasonic.distanceRead(), false);
  } else {
    digitalWrite(laserPin, LOW); 
    display.clear(); 
  }
  distance = ultrasonic.distanceRead();
  if (distance >= 100) { 
    digitalWrite(laserPin, HIGH); 
  } else {
    digitalWrite(laserPin, LOW);
  }
}
