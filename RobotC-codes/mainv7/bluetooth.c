#include "bluetooth.h"

long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime = 0;

const int kMaxSizeOfMessage = 30; //create a variable for the maximum message size
const int INBOX = 5;

task music();

void check_bluetooth(string *s) {
    /*
            This function makes a bleutooth connection and then waits for input from the connected phone
     */
    TFileIOResult nBTCmdRdErrorStatus;
    int nSizeOfMessage;
    ubyte nRcvBuffer[kMaxSizeOfMessage];
    //wait on bleutooth
    nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage) {//make the message shorter if its to long
    	nSizeOfMessage = kMaxSizeOfMessage;
    }

    if (nSizeOfMessage > 0) {//if the message is not empty, put the received information in string s
   		nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
      nRcvBuffer[nSizeOfMessage] = '\0';
      stringFromChars(*s, (char *) nRcvBuffer);
      displayCenteredBigTextLine(4, *s);
    }
}

int bluetooth_control(string *s) {
    /*
            This code is for taking over the robot completely using bleutooth to make sure you can stop de robot in case its needed.
            pressing the middle button ("FIRE") on the phone stops the robot due to it not being included in the code here.
            To get in to this function you have to press "B"
    */
    int stopcode = 0;

    while (*s != "A") {//if A is pressed the robot will resume its duty the normal way
        check_bluetooth(s);

        //The next 4 if statements are for controlling the robot manually
        if (*s == "LEFT") {//if bleutooth input is left turn left
            motor(motorA) = 0;
            motor(motorB) = 30;
            startTask(music); //make sure the music is running
        } else if (*s == "RIGHT") {//if bleutooth input is right turn right
            motor(motorA) = 30;
            motor(motorB) = 0;
            startTask(music);
        } else if (*s == "UP") {//if bleutooth input is up drive forward
            setMultipleMotors(50, motorB, motorA);
            startTask(music);
        } else if (*s == "DOWN") {//if bleutooth input is down drive backwards
            setMultipleMotors(-50, motorB, motorA);
            startTask(music);
        } else {//if there is no correct input turn off the motors
            setMultipleMotors(0, motorA, motorB);
            stopTask(music); //make sure the music is stopped
            clearSounds(); //empty the buffer
        }
        if (*s == "C") {//if the C button is pressed the loop stops and the stopcode == 1 to stop de code from running
            stopcode = 1;
            stopTask(music);
            clearSounds();
            break;
        }
    }
    *s = ""; //make sure s is empty
    return stopcode; //output of the function used to stop the code if chosen
}
