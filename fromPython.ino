#include <Servo.h>
Servo servoRoll;
Servo servoPitch;
Servo servoThumb;
Servo servoPointer;
Servo servoMiddle;
Servo servoRing;
Servo servoLittle;

String inNumber = "";
String inCode = "";
String inPole = "";
int angle = 90;
int rotation = 0;
String code = "";


void setup() 
{
  // assigning output pins to servos
  servoPitch.attach(3);
  servoRoll.attach(6);  
  servoThumb.attach(8);
  servoPointer.attach(9);
  servoMiddle.attach(10);
  servoRing.attach(11);
  servoLittle.attach(12);

  // initiating serial
  Serial.begin(115200);
  
  // setting the wrist servos at 90 degrees as starting (neutral) position
  servoRoll.write(angle);
  servoPitch.write(45);
  servoThumb.write(180);
  servoPointer.write(180);
  servoMiddle.write(180);
  servoRing.write(0);
  servoLittle.write(0);
}


void loop() 
{
  // while there is something in the serial, get the input
  while (Serial.available() > 0)
  { 
    // int because received in bytes
    int input = Serial.read();


    // if it is not a new line
    if (char(input) != 'n')
    {
      // if characther received is a number
      if (isDigit(input)) 
      {
        // convert the incoming byte to a char and add it to the string:
        inNumber += (char)input;
      }

      // if character received is a '-' update the inPole variable
      else if (input == 45)
      {
        inPole = "-";
      }
  
      // if character is a '.' ignore it
      else if (input == 46)
      {
        Serial.println("ignoring the dot");
      }
      
      else if (input == 82)
      {
        code = "R";         // WRIST ROLL
      }
      else if (input == 80)
      {
        code = "P";         // WRIST PITCH
      }
      else if (input == 65)
      {
        code = "A";         // THUMB
      }
      else if (input == 66)
      {
        code = "B";         // POINTER FINGER
      }
      else if (input == 67)
      {
        code = "C";         // MIDDLE FINGER
      }
      else if (input == 68)
      {
        code = "D";         // RING FINGER
      }
      else if (input == 69)
      {
        code = "E";         // LITTLE FINGER
      }
    }


    // when there is a new line
    else
    {
      // dividing by 10 to compensate for the decimal point
      angle = inNumber.toInt() / 10;

      // WRIST ROLL
      if (code == "R")
      {
        // if the angle received is bigger than 60, set the neutral servo position at 60 degrees - normal range
        if (angle > 60)
        {
          angle = 60;
        }

        // if the angle received is a negative number, deduct this angle from the neutral 90 degrees position
        // if the angle received is a positive number, add this angle to the neutral 90 degrees position
        if (inPole == "-")
        {        
          // if the angle is more than normal range, set it as the limit of the normal range for inward roll
          if (angle > 20)
          {
            angle = 20;
          }  
          rotation = 90 + angle;
        }
        else  //if 'inPole' is positive
        {
          rotation = 90 - angle;
        }
      
        int roll = map(rotation, 0, 180, 0, 180);
  
        servoRoll.write(roll);
      }


      // WRIST PITCH
      if (code == "P")
      {
        // if the angle received is bigger than 60, set the neutral servo position at 60 degrees - normal range
        if (angle > 45)
        {
          angle = 45;
        }

        // if the angle received is a negative number, deduct this angle from the neutral 90 degrees position
        // if the angle received is a positive number, add this angle to the neutral 90 degrees position
        if (inPole == "-")
        {
          rotation = 45 - angle;
        }
        else  //if 'inPole' is positive
        {
          rotation = 45 + angle;
        }
      
        int pitch = map(rotation, 0, 90, 0, 90);
  
        servoPitch.write(pitch);
      }


      // THUMB
      if (code == "A")
      {
        // neutral position of the thumb is when the servo is at angle 0 degrees
        // if the tip position is between 250 and 300, AND the palm position is at neutral, then map the finger's tip position to the angle of the servo
        // 250 - 300 is the range of the fingers tip distance from the LMC, but lessened by 50% for the Thumb
        if (angle > 250 && angle < 300)
        {
          int thumb = map(angle, 300, 200, 180, 140);
          
          servoThumb.write(thumb);
        }
      }


      // POINTER FINGER
      if (code == "B")
      {
        // neutral position of the finger is when the servo is at angle 0 degrees (servo inverted)
        // if the tip position is between 200 and 300, AND the palm position is at neutral, then map the finger's tip position to the angle of the servo
        // 200 - 300 is the range of the fingers tip distance from the LMC
        if (angle > 200 && angle < 300)
        {
          // mapping to 180..30 because the servo is attached upside down
          int pointer = map(angle, 300, 200, 180, 30);
          
          servoPointer.write(pointer);
        }
      }


      // MIDDLE FINGER
      if (code == "C")
      {
        // neutral position of the finger is when the servo is at angle 180 degrees (servo inverted)
        // if the tip position is between 200 and 300, AND the palm position is at neutral, then map the finger's tip position to the angle of the servo
        // 200 - 300 is the range of the fingers tip distance from the LMC
        if (angle > 200 && angle < 300)
        {
          // mapping to 180..30 because the servo is attached upside down
          int middle = map(angle, 300, 200, 180, 30);
          
          servoMiddle.write(middle);
        }
      }


      // RING FINGER
      if (code == "D")
      {
        // neutral position of the finger is when the servo is at angle 0 degreesd
        // if the tip position is between 200 and 300, AND the palm position is at neutral, then map the finger's tip position to the angle of the servo
        // 200 - 300 is the range of the fingers tip distance from the LMC
        if (angle > 200 && angle < 300)
        {
          int ring = map(angle, 300, 200, 0, 150);
          
          servoRing.write(ring);
        }
      }


      // LITTLE FINGER
      if (code == "E")
      {
        // neutral position of the finger is when the servo is at angle 0 degrees
        // if the tip position is between 200 and 300, AND the palm position is at neutral, then map the finger's tip position to the angle of the servo
        // 200 - 300 is the range of the fingers tip distance from the LMC
        if (angle > 200 && angle < 300)
        {
          int little = map(angle, 300, 200, 0, 150);
          
          servoLittle.write(little);
        }
      }


      // clearing the values of each variable, to repeate the process in the next loop
      inNumber = "";
      rotation = 0;
      inPole = "";
    }
  }

}
