# esp32-robotic-arm-atlas-project
!!! PROJECT STILL IN DEVELOPMENT !!!

A joystick-controlled robotic arm using an ESP32 and servo motors. Rotate and position joints using analog input and a button to switch between multiple degrees of freedom.

# ESP32 Robotic Arm Controller

A simple yet powerful robotic arm controller built with the ESP32, joystick, and 5 servo motors. The system allows you to control multiple joints of a robotic arm interactively using a joystick. Each press of the joystick's button cycles through different servo joints.

## Features

- Control 5 servo motors with just a joystick and ESP32
- Joystick button toggles control between joints
- Real-time analog input for smooth movement
- Adjustable claw controlled with a potentiometer
- Console feedback for angle debugging via Serial Monitor

## Hardware Requirements

- ESP32 development board
- 5x servo motors (4 180º SG90s and 1 360º SG90 for the base) 
- Joystick module (VRX, VRY, SW)
- Potentiometer (for claw control)
- External power supply for servos (recommended)
- Jumper wires and breadboard

## Pin Configuration

| Component       | ESP32 Pin |
|----------------|-----------|
| Servo 0 (Base)  | 23        |
| Servo 1 (Joint 1) | 18      |
| Servo 2 (Joint 2) | 19      |
| Servo 3 (Joint 3) | 15      |
| Servo 4 (Claw)  | 21        |
| Joystick VRX    | 34        |
| Joystick VRY    | 35        |
| Joystick Button (SW) | 32   |
| Claw Potentiometer  | 14    |

## How It Works

- **VRX and VRY** control the movement along two axes.
- **Joystick Button** (`SW`) toggles between controlling servo 1, 2, or 3.
- **Potentiometer** controls the claw’s open/close motion (servo 4).
- **Servo 0** moves based on the vertical Y-axis position.
- Servo angles are constrained to prevent over-rotation.
