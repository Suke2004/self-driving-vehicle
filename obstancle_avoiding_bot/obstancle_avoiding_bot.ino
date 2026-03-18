#include <Servo.h>
#include <NewPing.h>

#define TRIG_PIN 11
#define ECHO_PIN 12
#define MAX_DISTANCE 20

#define SERVO_PIN 3

// Motor pins
#define ENA 5
#define IN1 7
#define IN2 8

#define ENB 6
#define IN3 4
#define IN4 2

// Line sensors
#define LEFT_SENSOR A0
#define RIGHT_SENSOR A1

Servo myServo;
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int distance;
int leftSensor;
int rightSensor;

void setup()
{
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90);

  Serial.println("Robot Initialized");
  stopMotors();
}

void loop()
{
  distance = sonar.ping_cm();

  if(distance == 0)
  distance = 100;

  Serial.print("Distance: ");
  Serial.println(distance);

  // READ LINE SENSORS
  leftSensor = digitalRead(LEFT_SENSOR);
  rightSensor = digitalRead(RIGHT_SENSOR);

  Serial.print("Left Sensor: ");
  Serial.print(leftSensor);
  Serial.print(" | Right Sensor: ");
  Serial.println(rightSensor);

  // OBSTACLE DETECTED
  if(distance <= 20)
  {
    Serial.println("Obstacle Detected!");

    stopMotors();
    delay(1000);

    Serial.println("Turning Right");
    turnRight();
    delay(1000);

    stopMotors();
    delay(800);

    Serial.println("Moving Forward After Right Turn");
    forward();
    delay(1500);

    stopMotors();
    delay(800);

    Serial.println("Turning Left");
    turnLeft();
    delay(1000);

    stopMotors();
    delay(800);

    Serial.println("Moving Forward After Left Turn");
    forward();
    delay(1500);

    stopMotors();
    delay(800);

    turnLeft();
    delay(1000);

    stopMotors();
    delay(800);

    forward();
    delay(1500);

    stopMotors();
    delay(800);

    turnRight();
    delay(1000);

    stopMotors();
    delay(800);

    forward();
    delay(1500);



  }

  // NO OBSTACLE → LINE FOLLOWING
  else
  {
    Serial.println("Line Following Mode");

    if(leftSensor == LOW && rightSensor == LOW)
    {
      Serial.println("Moving Forward");
      forward();
    }

    else if(leftSensor == HIGH && rightSensor == LOW)
    {
      Serial.println("Adjusting Left");
      turnLeft();
    }

    else if(leftSensor == LOW && rightSensor == HIGH)
    {
      Serial.println("Adjusting Right");
      turnRight();
    }

    else
    {
      Serial.println("Stopping - No Line Detected");
      stopMotors();
    }
  }

  delay(200);
}

void forward()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);

  analogWrite(ENA,60);
  analogWrite(ENB,60);

  Serial.println("Motor: Forward");
}

void turnRight()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);

  analogWrite(ENA,70);
  analogWrite(ENB,70);

  Serial.println("Motor: Turn Right");
}

void turnLeft()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);

  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);

  analogWrite(ENA,70);
  analogWrite(ENB,70);

  Serial.println("Motor: Turn Left");
}

void stopMotors()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);

  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

  Serial.println("Motor: Stop");
}