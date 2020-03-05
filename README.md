# LMC-to-Robotic-Hand
Using Leap Motion Controller to mirror same movements to a robotic hand controlled with servos


python program (using python2.7) to read the data from the Leap Motion Controller (LMC)

data is passed via serial communication to Arduino

Arduino has attached 7 servos - one for each finger + two for wrist pitch & roll

Serial communication is sent via bluetooth from laptop to Arduino

The baudrate of the bluetooth HC05 module needed to be changed from 9600 to 115200 manually
