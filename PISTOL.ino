#include <TM1637Display.h>

const int buttonPin = 2;
const int laserPin = 7;
const int trigPin = 5;
const int echoPin = 6;

int distance;

#define CLK 4  
#define DIO 3  

TM1637Display display(CLK, DIO);

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); 
  pinMode(laserPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  digitalWrite(laserPin, LOW); 

  Serial.begin(9600);

  display.setBrightness(0x0F);  
  display.clear();              
}

void loop() {

  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { 

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    distance = pulseIn(echoPin, HIGH) / 58.2; 

    Serial.print("Расстояние: ");
    Serial.print(distance);
    Serial.println(" см");

    display.showNumberDec(distance, false);
  } else {
    digitalWrite(laserPin, LOW); 
    display.clear(); 
  }

  if (distance > 30) { 
    digitalWrite(laserPin, HIGH); 
  } else {
    digitalWrite(laserPin, LOW);
  }
}
