// connect motor controller pins to Arduino digital pins
// motor one
int enA = 5;    //D1
int in1 = 4;    //D2
int in2 = 2;    //D4

// motor two
int in3 = 13;   //D7    
int in4 = 12;   //D6
int enB = 14;   //D5

void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  drive(50, 50);
  delay(1000);
  drive(0, 0);
  delay(500);
  drive(50, -50);
  delay(1000);
  drive(0, 0);
  delay(500);
  drive(-50, 50);
  delay(1000);
  drive(0, 0);
  delay(500);
  drive(-50, -50);
  delay(1000);
  drive(0, 0);
  delay(500);
}

void drive(int speed_a, int speed_b){
  if (speed_a > 0 && speed_b > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }else if (speed_a < 0 && speed_b < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    speed_a *= -1;
    speed_b *= -1;
  } else if (speed_a > 0 && speed_b < 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    speed_b *= -1;
  } else if (speed_a < 0 && speed_b > 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);

    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    speed_a *= -1;
  } 
  
  if (speed_a == 0){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  if (speed_b == 0){
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  
  float s_a = speed_a * 1023 / 100.0;
  float s_b = speed_b * 1023 / 100.0;
  analogWrite(enA, s_a);
  analogWrite(enB, s_b);
}

