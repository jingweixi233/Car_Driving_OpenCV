//motor A connected between A01 and A02
//motor B connected between B01 and B02

int STBY = 10; //standby

//Motor A
int PWMA = 3; //Speed control 
int AIN1 = 9; //Direction
int AIN2 = 8; //Direction

//Motor B
int PWMB = 5; //Speed control
int BIN1 = 11; //Direction
int BIN2 = 12; //Direction

void setup(){
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  Serial.begin(9600);
}
int i;
void loop(){
  if (Serial.available()){
    i = Serial.read();

    switch(i){
      case'1':
      {turnLeft();break;}
      case'2':
      {forward();break;}
      case'3':
      {turnRight();break;}
      case'4':
      {turnLeftOrigin();break;}
      case'5':
      {stop();break;}
      case'6':
      {turnRightOrigin();break;}
      case'7':
      {turnbackLeft();break;}
      case'8':
      {back();break;}
      case'9':
      {turnbackRight();break;}
    }
  }
}


void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 forward, 1 back

  digitalWrite(STBY, HIGH); //disable standby

  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;

  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }

  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}

void forward()
{
  move(0, 255, 0);
  move(1, 255, 0);
}

void back()
{
  move(0, 255, 1);
  move(1, 255, 1);
}

void turnLeft()
{
  move(0, 128, 0);
  move(1, 0, 0);
}

void turnbackLeft()
{
  move(0, 128, 1);
  move(1, 0, 0);
}

void turnRight()
{
  move(1, 128, 0);
  move(0, 0, 0);
}

void turnbackRight()
{
  move(1, 128, 1);
  move(0, 0, 0);
}

void turnLeftOrigin()
{
  move(0, 128, 0);
  move(1, 128, 1);
}

void turnRightOrigin()
{
  move(1, 128, 0);
  move(0, 128, 1);
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}
