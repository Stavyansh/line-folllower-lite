# Line Follower Robot

Stavyansh Mishra  
Team Vyadh

## Overview

This project implements a simple differential drive line follower using an Arduino Uno, two IR sensors, and an L298N motor driver. The robot follows a black line on a white surface by adjusting motor direction based on sensor feedback.

The system relies on the difference in infrared reflectivity.

- white surfaces reflect more IR radiation  
- black surfaces absorb IR radiation  

The IR sensors are calibrated so that:

- white surface → LOW output  
- black line → HIGH output  

Two sensors are placed at the front of the robot with a small separation so the robot can detect when it deviates from the line.

---

## Control Behaviour

| Left Sensor | Right Sensor | Left Motor | Right Motor |
|-------------|-------------|-----------|-------------|
| LOW | LOW | Forward | Forward |
| HIGH | LOW | Backward | Forward |
| LOW | HIGH | Forward | Backward |
| HIGH | HIGH | Stop | Stop |

Explanation

- LOW LOW means the robot is centered on the line and moves forward.
- HIGH LOW means the left sensor detects the line so the robot turns left.
- LOW HIGH means the right sensor detects the line so the robot turns right.
- HIGH HIGH means both sensors detect black and the robot stops.

Turning is achieved by rotating the motors in opposite directions.

---

## Hardware Connections

Power

2 × 3.7V Li-ion batteries power the L298N motor driver.

System structure

Arduino → L298N → Motors → Wheels  
Arduino → IR sensors

Arduino to L298N

| Arduino Pin | L298N Pin | Function |
|--------------|-----------|---------|
| D6 | ENA | Right motor speed (PWM) |
| D7 | IN1 | Right motor direction |
| D8 | IN2 | Right motor direction |
| D5 | ENB | Left motor speed (PWM) |
| D9 | IN3 | Left motor direction |
| D10 | IN4 | Left motor direction |

Sensors

| Arduino Pin | Sensor |
|--------------|--------|
| D11 | Right IR sensor |
| D12 | Left IR sensor |

---

# Code Explanation

## 1. Pin configuration and speed constant

```
int irRight = 11;
int irLeft = 12;

int enRight = 6;
int in1Right = 7;
int in2Right = 8;

int enLeft = 5;
int in1Left = 9;
int in2Left = 10;

int speed = 175;
```
These variables define all the pins connected to the sensors and the L298N driver.

irRight and irLeft store the digital input pins for the IR sensors.

enRight and enLeft are PWM pins used to control motor speed.

in1/in2 pairs control motor direction.

The speed variable defines the PWM duty cycle applied to both motors.

2. Timer configuration and system initialization
```
TCCR0B = (TCCR0B & 0b11111000) | 0b00000010;
```
This line modifies the Timer0 configuration of the ATmega328P.

TCCR0B is a register that controls Timer0.

Timer0 generates PWM signals for pins 5 and 6, which are connected to the L298N enable pins.

(TCCR0B & 0b11111000) clears the last three bits of the register.
0b00000010 sets a new prescaler value.

This effectively increases the PWM frequency, which helps the DC motors run more smoothly and reduces jerky motion.

3. Pin initialization
```
pinMode(irRight, INPUT);
pinMode(irLeft, INPUT);

pinMode(enRight, OUTPUT);
pinMode(in1Right, OUTPUT);
pinMode(in2Right, OUTPUT);

pinMode(enLeft, OUTPUT);
pinMode(in1Left, OUTPUT);
pinMode(in2Left, OUTPUT);

moveMotors(0,0);
```
Here the Arduino configures all sensor pins as inputs and all motor driver pins as outputs.

moveMotors(0,0) ensures that both motors remain stopped during system startup.

4. Sensor reading and decision logic
```
int right = digitalRead(irRight);
int left = digitalRead(irLeft);
```
The Arduino continuously reads both IR sensors.

The following logic determines the robot’s motion.
```
if (right == LOW && left == LOW) {
    moveMotors(speed, speed);
}
```
Both sensors detect white so the robot moves forward.
```
else if (right == HIGH && left == LOW) {
    moveMotors(speed, -speed);
}
```
Left sensor detects the line so the robot turns left.
```
else if (right == LOW && left == HIGH) {
    moveMotors(-speed, speed);
}
```
Right sensor detects the line so the robot turns right.
```
else {
    moveMotors(0,0);
}
```
If both sensors detect black the robot stops.

5. Motor control function
```
void moveMotors(int rightSpeed, int leftSpeed)
```
This user defined function controls both motors.

The sign of the speed value determines the direction.

Example for the right motor:
```
if (rightSpeed > 0) {
    digitalWrite(in1Right, HIGH);
    digitalWrite(in2Right, LOW);
}
```
Positive speed → forward rotation.
```
else if (rightSpeed < 0) {
    digitalWrite(in1Right, LOW);
    digitalWrite(in2Right, HIGH);
}
```
Negative speed → reverse rotation.

Motor speed is controlled using PWM.
```
analogWrite(enRight, abs(rightSpeed));
```
The abs() function converts negative values to positive because PWM values must always be positive.

The same logic is applied to the left motor.

Speed Selection

The speed value was determined experimentally, values below ~175 caused the motors to stall or only hum

higher values caused the robot to overshoot while turning

A value around 175–180 provided stable tracking.

## Observed Limitations

Some issues observed during testing:

motor lag due to hobby DC motors

sensor alignment sensitivity

uneven weight distribution from batteries

occasional motor delay when battery voltage drops

wheel misalignment causing wobble

These factors introduce small tracking errors but the robot performs reliably in most cases.
