//Pin assignment
#define lSensor 2 //left sensor input
#define rSensor 3 //right sensor input

#define lMotPos 8  //left motor(+)
#define lMotNeg 7  //left motor(-)

#define rMotNeg 13  //right motor (-)
#define rMotPos 12 //right motor (+)

#define lMotPWM 6 //left motor speed(pwm) pin
#define rMotPWM 5 //right motor speed(pwm) pin

int lMotSpeed = 180;
int rMotSpeed = 180;
int lTurnSpeed= 150;
int rTurnSpeed = 150;

void setup()
{
  TCCR0B = TCCR0B & B11111000 | B00000010 ;
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
  
  analogWrite(lMotPWM, lMotSpeed);
  analogWrite(rMotPWM, rMotSpeed);
  Serial.begin(9600);

  delay(100);
}

void loop()
{  

  int lSensStat = digitalRead(lSensor);
  int rSensStat = digitalRead(rSensor);
  if (!lSensStat && !rSensStat) //both sensors detected white floor. go straight or check side
  {
    forward();
    Serial.println("forward");
  }
  if (!lSensStat && rSensStat) //detected black floor on right (off), so turn right 
  {
    turnRight();
    Serial.println("right");
  }

  if (lSensStat && !rSensStat) //detected black floor on left (off), so turn left
  {
    turnLeft();
    Serial.println("left");
  }

  if (lSensStat && rSensStat) //detected black floor on both sensor, so stop!
  {
    stop();
    Serial.println("stop");
  }

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
  analogWrite(lMotPWM, lMotSpeed);   
   
  //set right motor control parameters
  digitalWrite (rMotNeg, HIGH);    
  digitalWrite (rMotPos, LOW);
  analogWrite(rMotPWM, 1.4*rMotSpeed);
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
