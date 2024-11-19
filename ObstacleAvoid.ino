//Pin assignment
#define lSensor 2 //left sensor input
#define rSensor 3 //right sensor input

#define lMotPos 8  //left motor(+)
#define lMotNeg 7  //left motor(-)

#define rMotNeg 13  //right motor (-)
#define rMotPos 12 //right motor (+)

#define lMotPWM 6 //left motor speed(pwm) pin
#define rMotPWM 5 //right motor speed(pwm) pin

#define echo A4    //Echo pin
#define trigger A3 //Trigger pin

#define servoPin 9 //Servo pin

int set = 15;
int distance_L, distance_F, distance_R;

int lMotSpeed = 150;
int rMotSpeed = 150;
int lTurnSpeed= 140;
int rTurnSpeed = 140;

void setup()
{
  //IR sensor pins
  pinMode (lSensor,INPUT);
  pinMode (rSensor,INPUT);
  
  //L298 driver pins
  pinMode (lMotNeg, OUTPUT);
  pinMode (lMotPos, OUTPUT);
  pinMode (lMotPWM, OUTPUT);

  pinMode (rMotNeg, OUTPUT);
  pinMode (rMotPos, OUTPUT);
  pinMode (rMotPWM, OUTPUT);

  pinMode (echo, INPUT);
  pinMode (trigger, OUTPUT);
  
  analogWrite(lMotPWM, lMotSpeed);
  analogWrite(rMotPWM, rMotSpeed);

  stop();
  delay(100);
  
  servoPulse(servoPin, 55);
  
  Serial.begin(9600);

  delay(100);
}

void loop()
{  
  // delay(40);
  int lSensStat = digitalRead(lSensor);
  int rSensStat = digitalRead(rSensor);
  distance_F = Ultrasonic_read();
  distance_F = distance_F == 0 ? 30: distance_F;

  Serial.println(distance_F);

  if (!lSensStat && !rSensStat) //both sensors detected white floor. go straight or check side
  {
    if(distance_F > set)
    {
      forward();
    }
    else
    {
      turnAround(lSensStat);
    }
  }
  if (!lSensStat && rSensStat) //detected black floor on right (off), so turn right 
  {
    turnRight();
  }

  if (lSensStat && !rSensStat) //detected black floor on left (off), so turn left
  {
    turnLeft();
  }

  if (lSensStat && rSensStat) //detected black floor on both sensor, so stop!
  {
    backward();
  }
}

void servoPulse (int pin, int angle)
{
  int pwm = (angle*11) + 500;      // Convert angle to microseconds
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(50); // Refresh cycle of servo
}

long Ultrasonic_read()
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}



void turnAround(int lSensStat)
{
  stop();
  delay(300);
  lTurnSpeed = 130;
  while(!lSensStat)
  {
    lSensStat = digitalRead(lSensor);
    turnLeft();
  }
  lTurnSpeed = 150;
  stop();
  delay(300);
}

void forward ()
{
  //set left motor control parameter
  digitalWrite (lMotNeg, LOW);
  digitalWrite (lMotPos, HIGH);
  analogWrite(lMotPWM, lMotSpeed);   
   
  //set right motor control parameters
  digitalWrite (rMotNeg, LOW);    
  digitalWrite (rMotPos, HIGH);
  analogWrite(rMotPWM, rMotSpeed);
}

void backward ()
{
  //set left motor control parameter
  digitalWrite (lMotNeg, HIGH);
  digitalWrite (lMotPos, LOW);
  analogWrite(lMotPWM, 1.5*lMotSpeed);   
   
  //set right motor control parameters
  digitalWrite (rMotNeg, HIGH);    
  digitalWrite (rMotPos, LOW);
  analogWrite(rMotPWM, 1.5*rMotSpeed);
}

void turnRight ()
{
  digitalWrite (lMotNeg, LOW);    //left motor forward
  digitalWrite (lMotPos, HIGH);
  analogWrite(lMotPWM, lTurnSpeed);
  
  digitalWrite (rMotNeg, HIGH);    //right motor reverse
  digitalWrite (rMotPos, LOW);
  analogWrite(rMotPWM, rTurnSpeed);
}

void turnLeft ()
{
  digitalWrite (lMotNeg, HIGH);  //left motor reverse
  digitalWrite (lMotPos, LOW);
  analogWrite(lMotPWM, lTurnSpeed);
  
  digitalWrite (rMotNeg, LOW);    //right motor forward
  digitalWrite (rMotPos, HIGH);
  analogWrite(rMotPWM, rTurnSpeed);
}

void stop() 
{
  digitalWrite (lMotNeg, LOW);
  digitalWrite (lMotPos, LOW);
  digitalWrite (rMotNeg, LOW);
  digitalWrite (rMotPos, LOW);
}
