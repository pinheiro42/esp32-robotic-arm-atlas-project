#include <ESP32Servo.h>

#define s0pin 23
#define s1pin 18
#define s2pin 19
#define s3pin 15
#define s4pin 21

#define vrx_pin 34
#define vry_pin 35
#define sw_pin 32
#define claw_pin 14

int counter = 0, angle4 = 0;
bool lastButtonState = LOW;

int angle1 = 90, angle2 = 35, angle3 = 90;

Servo s0, s1, s2, s3, s4;

void setup() {
  Serial.begin(115200);
  pinMode(sw_pin, INPUT_PULLUP);
  pinMode(claw_pin, INPUT);

  s0.attach(s0pin);
  s1.attach(s1pin);
  s2.attach(s2pin);
  s3.attach(s3pin);
  s4.attach(s4pin);

  s0.write(90);
  s1.write(90);
  s2.write(90);
  s3.write(90);
  s4.write(0);

  delay(500);
}

void loop() {
  int vrx = analogRead(vrx_pin);
  int vry = analogRead(vry_pin);
  int claw = analogRead(claw_pin);
  int buttonState = digitalRead(sw_pin);

  angle4 = map(claw, 0, 4095, 0, 160);
  s4.write(angle4);

  if (vry > 2000) {
    s0.write(60);
  } else if (vry < 1200) {
    s0.write(130);
  } else {
    s0.write(90);
  }

  if (buttonState == HIGH && lastButtonState == LOW) {
    counter = (counter + 1) % 3;
    delay(100);
  }
  lastButtonState = buttonState;
  //Serial.println(counter);
  Serial.println(claw);

  switch (counter) {
    case 0: ServoControl1(vrx); break;
    case 1: ServoControl2(vrx); break;
    case 2: ServoControl3(vrx); break;
  }
}

void ServoControl1(int vrx) {
  if (vrx > 2000) {
    angle1--;
    delay(10);
  } else if (vrx < 1400) {
    angle1++;
    delay(10);
  }
  angle1 = constrain(angle1, 51, 130);
  s1.write(angle1);
  Serial.println(angle1);
}

void ServoControl2(int vrx) {
  if (vrx > 2000) {
    angle2--;
    delay(10);
  } else if (vrx < 1400) {
    angle2++;
    delay(10);
  }
  angle2 = constrain(angle2, 0, 130);
  s2.write(angle2);
  Serial.println(angle2);
}

void ServoControl3(int vrx) {
  if (vrx > 2000) {
    angle3--;
    delay(10);
  } else if (vrx < 1400) {
    angle3++;
    delay(10);
  }
  angle3 = constrain(angle3, 0, 130);
  s3.write(angle3);
  Serial.println(angle3);
}
