// Motor Driver Pins
#define ENA 5
#define IN1 7
#define IN2 8

#define ENB 6
#define IN3 4
#define IN4 2

// Sensors
#define LEFT_SENSOR A0
#define RIGHT_SENSOR A1

int leftSensor;
int rightSensor;

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
}

void loop()
{

  leftSensor = digitalRead(LEFT_SENSOR);
  rightSensor = digitalRead(RIGHT_SENSOR);

  // WHITE WHITE → FORWARD
  if(leftSensor == LOW && rightSensor == LOW)
  {
    forward();
  }

  // LEFT BLACK → TURN LEFT
  else if(leftSensor == HIGH && rightSensor == LOW)
  {
    turnLeft();
  }

  // RIGHT BLACK → TURN RIGHT
  else if(leftSensor == LOW && rightSensor == HIGH)
  {
    turnRight();
  }

  // BOTH BLACK → STOP
  else
  {
    stopMotors();
  }

}

void forward()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 80);
  analogWrite(ENB, 80);
}

void turnLeft()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 80);
  analogWrite(ENB, 80);
}

void turnRight()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 80);
  analogWrite(ENB, 80);
}

void stopMotors()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}