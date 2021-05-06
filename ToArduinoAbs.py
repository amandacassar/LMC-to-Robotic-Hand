import serial, time, Leap, sys, thread, time, math

ser = serial.Serial('COM8', 115200, timeout=1)


class LeapMotionListener(Leap.Listener):
    finger_names = ['A', 'B', 'C', 'D', 'E']
    bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']
    state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']

    # def = function // self = this
    def on_init(self, controller):
        print ("Initialized")

    def on_connect(self, controller):
        print("Motion Sensor Connected")
        
    def on_disconnect(self, controller):
        print ("Motion Sensor Disconnected")

    def on_exit(self, controller):
        print ("Exited")

    def on_frame(self, controller):
        frame = controller.frame()

        try:
            ser.isOpen()
        except:
            print("ERROR #1")
            exit()

        for hand in frame.hands:
            
            normal = hand.palm_normal       
            direction = hand.direction      

            if (ser.isOpen()):
                try:
                    # obtainting the radian value for the writs pitch and roll - and converting the radian value into degrees
                    # adding text to it, that will allow Arduino to identify the command
                    pitch = "P" + str(math.ceil(direction.pitch * Leap.RAD_TO_DEG))
                    roll = "R" + str(math.ceil(normal.roll * Leap.RAD_TO_DEG))

                    # obtaining the pitch and roll degree angle - needed for fingers position
                    pDegree = math.ceil(direction.pitch * Leap.RAD_TO_DEG)
                    rDegree = math.ceil(normal.roll * Leap.RAD_TO_DEG)

                    # initialising Arduino - declaring a delay of 10ms
                    time.sleep(0.01)

                    # printing in terminal and passing through serial the wrist pitch in degrees - bytes format - as interpreted by Arduino serial
                    # a new line ('\n') is added at the end of the serial data, since that is how Arduino Ide will recognise the end of a command
                    print(bytes(pitch) + 'n')
                    ser.write(bytes(pitch) + 'n')
                    ser.flush()                     

                    # printing in terminal and passing through serial the wrist roll in degrees - bytes format - as interpreted by Arduino serial
                    print(bytes(roll) + 'n')
                    ser.write(bytes(roll) + 'n')
                    ser.flush()

                    
                    # condition to not allow the robotic fingers to move when the wrist is pitching or rolling (even if finger tip position still changes)
                    # if the absolute value of the wrist pitch or the wrist roll is bigger than 10, do not move the fingers
                    if ((abs(pDegree) < 10) and (abs(rDegree) < 20)):

                        # note - fingers indices in Leapmotion are: 0-thumb; 1-pointer; 2-middle; 3-ring; 4-little
                        # finger.type() returns the finger index (as per above line of comments)
                        for finger in hand.fingers:
                            # obtainting the y-axis value for each finger tip - y being the vertical axis
                            finger = (self.finger_names[finger.type] + str(math.ceil(finger.stabilized_tip_position.y)))

                            # printing in terminal and passing through serial the fingers' tips y-position - bytes format - as interpreted by Arduino serial
                            # a new line ('\n') is added at the end of the serial data, since that is how Arduino Ide will recognise the end of a command
                            print(bytes(finger) + 'n')
                            ser.write(bytes(finger) + 'n')

                except Exception, err:
                    print Exception, err



def main():
    listener = LeapMotionListener()
    controller = Leap.Controller()

    controller.add_listener(listener)

    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        controller.remove_listener(listener)

if __name__ == "__main__":
    main()