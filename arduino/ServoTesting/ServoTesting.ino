#include <Servo.h>

Servo servo;

int pos = 0;
int leftButton = 2;
int rightButton = 3;


void setup() {  
  servo.attach(9);  
  Serial.begin(9600);
  pinMode(leftButton, INPUT);
  pinMode(rightButton, INPUT);
}

void loop() {
  int leftButtonState = digitalRead(leftButton);
  int rightButtonState = digitalRead(rightButton);

  if (leftButtonState == 1) {
    Serial.println("LEFT");
    if (pos >= 0) {
      pos -= 1;
    }
  }
  if (rightButtonState == 1) {
    Serial.println("RIGHT");
    if (pos <= 180) {
      pos +=1 ;
    }
  }

  servo.write(pos);
  delay(10);
}

