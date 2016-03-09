// connect motor controller pins to Arduino digital pins
// motor one
int enA = 14;
int in1 = 13;
int in2 = 12;

// motor two
int enB = 2;
int in3 = 5;
int in4 = 4;
double motorSpeed = 0.7;
void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
void demoOne()
{
  // this function will run the motors in both directions at a fixed speed
  // turn on motor A
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enA, 1023*motorSpeed);
  // turn on motor B
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  // set speed to 200 out of possible range 0~255
  analogWrite(enB, 1023*motorSpeed);
  delay(1000);
  
  analogWrite(enA, 1023*0);
  analogWrite(enB, 1023*0);
  delay(1000);
  // now change motor directions
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
  delay(1000);
  analogWrite(enA, 1023*0.7);
  analogWrite(enB, 1023*0.7);
  delay(1000);
  // now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void loop()
{
  demoOne();
  delay(1000);
}
