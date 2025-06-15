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

int angle1 = 61, angle2 = 11, angle3 = 71, angle4 = 95;

float posX = 6.0;   // Initial X position (adjustable)
float posY = 0.0;    // Initial Y position (adjustable)

float L1 = 9.44;     // Length of first arm segment
float L2 = 6.44;     // Length of second arm segment
float L3 = 7.92;     // Length of third segment (not used in IK)

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
  // s1.write(angle1);
  // s2.write(angle2);
  s3.write(angle3);
  s4.write(angle4);

  delay(500);
}

void loop() {
  int vrx = analogRead(vrx_pin);
  int vry = analogRead(vry_pin);
  int claw = analogRead(claw_pin);

  // Claw control
  angle4 = map(claw, 0, 4095, 0, 160);
  s4.write(angle4);

  // Joystick control
  if (vrx > 2000) posX -= 0.1;
  else if (vrx < 1400) posX += 0.1;

  if (vry > 2000) posY -= 0.1;
  else if (vry < 1400) posY += 0.1;

  posX = constrain(posX, -4.0, 14.0);
  posY = constrain(posY, 0.0, 14.0);
  // Keep within reach
  float maxReach = L1 + L2 - 0.5;
  float dist = sqrt(posX * posX + posY * posY);
  if (dist > maxReach) {
    float scale = maxReach / dist;
    posX *= scale;
    posY *= scale;
  }

  // Inverse Kinematics
  float t1, t2;
  if (solveIK(posX, posY, t1, t2)) {
    // Normalize t1 to [-180°, 180°]
    t1 = fmod(t1 + 360.0, 360.0);
    if (t1 > 180.0) t1 -= 360.0;

    // Servo angle 1 (base)
    int theta1_servo = map(t1, -90, 90, 40, 140);
    theta1_servo = constrain(theta1_servo, 40, 140);

    // Servo angle 2 (elbow - inverted V)
    int theta2_servo = map(t2, 0, 150, 130, 10);
    theta2_servo = constrain(theta2_servo, 10, 130);

    s1.write(theta1_servo);
    s2.write(theta2_servo);

    Serial.print("angle 1: "); Serial.print(theta1_servo);
    Serial.print(" | angle 2: "); Serial.print(theta2_servo);
    Serial.print(" | posX: "); Serial.print(posX);
    Serial.print(" | posY: "); Serial.print(posY);
    Serial.print(" | t1: "); Serial.print(t1);
    Serial.print(" | t2: "); Serial.println(t2);
  } else {
    Serial.println("Unreachable target!");
  }

  delay(15);
}


bool solveIK(float x, float y, float& theta1, float& theta2) {
  float dist = sqrt(x * x + y * y);
  if (dist > (L1 + L2)) return false;

  float cos_theta2 = (x * x + y * y - L1 * L1 - L2 * L2) / (2 * L1 * L2);
  if (cos_theta2 < -1 || cos_theta2 > 1) return false;

  theta2 = acos(cos_theta2);  // radians
  float k1 = L1 + L2 * cos(theta2);
  float k2 = L2 * sin(theta2);
  theta1 = atan2(y, x) - atan2(k2, k1);

  // Convert to degrees
  theta1 = degrees(theta1);
  theta2 = degrees(theta2);

  return true;
}

