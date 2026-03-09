int irRight = 11;
int irLeft = 12;
int enRight = 6;
int in1Right = 7;
int in2Right = 8;
int enLeft = 5;
int in1Left = 9;
int in2Left = 10;
int speed = 175;
void setup() {
  TCCR0B=(TCCR0B & 0b11111000) | 0b00000010;
  pinMode(irRight, INPUT); 
  pinMode(irLeft, INPUT);
  pinMode(enRight, OUTPUT);
  pinMode(in1Right, OUTPUT);
  pinMode(in2Right, OUTPUT);
  pinMode(enLeft, OUTPUT);
  pinMode(in1Left, OUTPUT);
  pinMode(in2Left, OUTPUT);
  moveMotors(0,0);
 
}
void loop() {
  int right = digitalRead(irRight); 
  int left = digitalRead(irLeft);
  if (right == LOW && left == LOW) {
    moveMotors(speed, speed);         
  } else if (right == HIGH && left == LOW) {
    moveMotors(speed, -speed);     
  } else if (right == LOW && left == HIGH) {
    moveMotors(-speed, speed);        
  } else {
    moveMotors(0, 0);                 
  }
}

void moveMotors(int rightSpeed, int leftSpeed) {
  if (rightSpeed > 0) {
    digitalWrite(in1Right, HIGH);
    digitalWrite(in2Right, LOW);
  } else if (rightSpeed < 0) {
    digitalWrite(in1Right, LOW);
    digitalWrite(in2Right, HIGH);
  } else {
    digitalWrite(in1Right, LOW);
    digitalWrite(in2Right, LOW);
  }
  analogWrite(enRight, abs(rightSpeed));

  if (leftSpeed > 0) {
    digitalWrite(in1Left, HIGH);
    digitalWrite(in2Left, LOW);
  } else if (leftSpeed < 0) {
    digitalWrite(in1Left, LOW);
    digitalWrite(in2Left, HIGH);
  } else {
    digitalWrite(in1Left, LOW);
    digitalWrite(in2Left, LOW);
  }
  analogWrite(enLeft, abs(leftSpeed));
}
