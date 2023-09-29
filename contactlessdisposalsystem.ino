// This code is hard coded for a path towards the bigger trashcan, with the hardware and the desired path, the path planning algorithm will be implemented
// Constants for pin assignments
const int trigger1 = 2;
const int echo1 = 3;
const int trigger2 = 4;
const int echo2 = 5;
const int servoPin = 9;
const int S0 = 6;
const int S1 = 7;
const int S2 = 8;
const int S3 = 10;
const int OUT = 11;
const int in1 = 13;
const int in2 = 14;
const int in3 = 15;
const int in4 = 16;

int flag = 1; // Flag for control flow
long duration1, distance1, duration2, distance2; // Ultrasonic sensors variables

void setup()
{
  // Configure pin modes for various components
  pinMode(servoPin, OUTPUT); // Servo control pin
  pinMode(trigger1, OUTPUT); // Ultrasonic sensor 1 trigger
  pinMode(echo1, INPUT);     // Ultrasonic sensor 1 echo
  pinMode(trigger2, OUTPUT); // Ultrasonic sensor 2 trigger
  pinMode(echo2, INPUT);     // Ultrasonic sensor 2 echo
  pinMode(S0, OUTPUT);       // Color sensor S0
  pinMode(S1, OUTPUT);       // Color sensor S1
  pinMode(S2, OUTPUT);       // Color sensor S2
  pinMode(S3, OUTPUT);       // Color sensor S3
  pinMode(OUT, INPUT);       // Color sensor OUT
  pinMode(in1, OUTPUT);      // Motor driver input 1
  pinMode(in2, OUTPUT);      // Motor driver input 2
  pinMode(in3, OUTPUT);      // Motor driver input 3
  pinMode(in4, OUTPUT);      // Motor driver input 4
  
  // Initialize color sensor pins
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
}

void loop()
{
  // Ultrasonic sensor 1 for proximity detection of the person with waste coming towards the bin
  digitalWrite(trigger1, LOW); // Start a new measurement
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  duration1 = pulseIn(echo1, HIGH); // Measure the duration of the echo pulse
  distance1 = (duration1 / 2) / 29.1; // Convert duration to distance in cm

  // Ultrasonic sensor 2 for waste level detection
  digitalWrite(trigger2, LOW); // Start a new measurement
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  duration2 = pulseIn(echo2, HIGH); // Measure the duration of the echo pulse
  distance2 = (duration2 / 2) / 29.1; // Convert duration to distance in cm

  // Print sensor readings to serial monitor
  Serial.print("Distance from Sensor 1: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Distance from Sensor 2: ");
  Serial.print(distance2);
  Serial.println(" cm");

  // Control servo to open/close lid based on proximity
  rotateServo(0); // The lid is closed
  if (distance1 < 10) // If a person is detected within this range
  {
    rotateServo(90); // Open the lid
    delay(10000); // Keep the lid open for 10 seconds
    rotateServo(0); // Close the lid
  }

  // Check if waste level is high and initiate path planning
  if (distance2 < 5)
  {
    pathplanning(); // Implement path planning for navigation
  }

  // Determine color of the bigger trashcans using the color sensor
  int redValue = getColor(LOW, LOW);    // Reading color intensity with both red and blue are not present
  int blueValue = getColor(HIGH, HIGH);  // Reading color intensity with both red and blue present

  // If redValue is less than blueValue, it means red color is detected
  if (redValue < blueValue)
  {
    // Rotate the lid servo to dump the waste
    rotateServo(110); // Open the lid
    delay(15000); // Keep the lid open for 15 seconds
    rotateServo(0); // Close the lid
    flag = 0; // Set flag to indicate that dumping has occurred
  }

  // If the flag is set (dumping has occurred), perform traceback
  if (flag == 0)
  {
    traceback(); // Execute the traceback logic
  }
}

// Function to get the color intensity using the color sensor
int getColor(int s2State, int s3State)
{
  // Set the S2 and S3 states for the given color
  digitalWrite(S2, s2State);
  digitalWrite(S3, s3State);

  // Read the color intensity on the OUT pin
  return pulseIn(OUT, LOW);
}

// Function to rotate the servo to a specified degree
void rotateServo(int degrees)
{
  int pulseWidth = map(degrees, 0, 180, 544, 2400);
  digitalWrite(servoPin, HIGH); // Activate servo
  delayMicroseconds(pulseWidth);
  digitalWrite(servoPin, LOW); // Deactivate servo
}

// Function for path planning
void pathplanning()
{
  front();
  delay(5000);
  right();
  delay(1000);
  front();
  delay(3000);
  left();
  delay(1000);
  front();
  delay(5000);
  right();
  delay(1000);
  stop();
}

// Function for traceback
void traceback()
{
  right();
  delay(1000);
  front();
  delay(5000);
  right();
  delay(1000);
  front();
  delay(3000);
  left();
  delay(1000);
  front();
  delay(5000);
}

// Helper functions for controlling motor movement
void front()
{
  digitalWrite(in1, HIGH); // Motor: Move forward
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void back()
{
  digitalWrite(in1, LOW); // Motor: Move backward
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right()
{
  digitalWrite(in1, LOW); // Motor: Turn right
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void left()
{
  digitalWrite(in1, HIGH); // Motor: Turn left
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void stop()
{
  digitalWrite(in1, LOW); // Motor: Stop
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}