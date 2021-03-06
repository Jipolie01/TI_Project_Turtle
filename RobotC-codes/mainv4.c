#pragma config(Sensor, S1,     color_sensor,   sensorColorNxtRED)
#pragma config(Sensor, S2,     light_sensor,   sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          motor_right,   tmotorNXT, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          motor_left,    tmotorNXT, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(NXT)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;

const int kMaxSizeOfMessage = 30;//create a variable for the maximum message size
const int INBOX = 5;

void speed_adjust(int index, int until, int increment){
	/*
		This function is used for slowing down the motor slowly instead of stopping immediatly
	*/
	for (int i = index; i >= until; i+=increment){
		setMultipleMotors(i, motorA, motorB);
		wait(0.09);
	}
}

void check_bleutooth(string *s){
	/*
		This function makes a bleutooth connection and then waits for input from the connected phone
	*/
	TFileIOResult nBTCmdRdErrorStatus;
  int nSizeOfMessage;
  ubyte nRcvBuffer[kMaxSizeOfMessage];

	while(1){
		//wait on bleutooth
		 nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage){//make the message shorter if its to long
      nSizeOfMessage = kMaxSizeOfMessage;
    }

    if (nSizeOfMessage > 0){//if the message is not empty, put the received information in string s
    	nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    	nRcvBuffer[nSizeOfMessage] = '\0';
    	stringFromChars(*s, (char *) nRcvBuffer);
    	displayCenteredBigTextLine(4, *s);
    	break;
    }

    wait1Msec(100);//no need to do a continues check every 100 miliseconds is enough
	}
}

task music(){
	//plays music while robot is in motion
	playSoundFile("Supermario.rmd");
	wait1Msec(10000);
}

void linefollow(void){
	//using float variable for the sensors to have the most 	accurate
	float color;
	float light;
	float sonar;

	// reading the sensor value
	light = SensorValue[S2];
	color = SensorValue[S1];
	sonar = SensorValue[S3];

	// displaying the values from above
	nxtDisplayTextLine(1, "color: %d", color);
	nxtDisplayTextLine(2, "light: %d", light);
	nxtDisplayTextLine(3, "sonar: %d", sonar );

	// if an object is closer then 30 centimeters do the folowing
	if(sonar < 30) {
		speed_adjust(60, 0, -4);//slow down

		for (int i = 0; i < 10;){//slowly start turning
			motor[motorA] = i;
			motor[motorB] = -i;
			wait(0.4);
			i++;
		}

		while(1){//wait until you see the line
			if (SensorValue[S2] < 60){
				for (int i = 10; i < 0;){//slow down the turning speed
					motor[motorA] = i;
					motor[motorB] = -i;
					wait(0.4);
					i+=2;
				}
				break;
			}
		}
	}

	//redirect the robot by using the light sensor
	else if (light < 69){
		float formule;
		float omrek;
		float formule2;
		omrek = light-39;
		nxtDisplayTextLine(4, "lomrek : %d", omrek);

		if(omrek<5){
			formule =100;
			formule2=0;
		}
		else if (omrek<10){
			formule = -0.4*omrek+100;
			formule2 = -1.428*omrek+50;
		}
		else{
			formule = -0.9*omrek+100;
			formule2 = -1.428*omrek+50;
		}

		//startTask(music);//starts the music
		motor(motorA) = formule;//50+((60-light)*2.5)
		motor(motorB) =formule2;//35  //0
	}

	// redirecting the robot by using hte color sensor
	else if(color < 51) {//vorige waarde: 4
		// float variables for best accurate values
		float formule;
		float omrek;
		float formule2;

		omrek = color-22;
		nxtDisplayTextLine(5, "comrek : %d", omrek);

		if(omrek<5)
		{
			formule =100;
			formule2=0;
		}

		else if (omrek<10)
		{
			formule = -0.4*omrek+100;
			formule2 = -1.428*omrek+50;
		}

		else {
			formule = -0.9*omrek+100;
			formule2 = -1.428*omrek+50;
		}

		startTask(music);//starts the music
		motor(motorA) = formule2;//0
		motor(motorB) =formule ;//50+(50-color)*1+(2/3))
	}
	else {
		setMultipleMotors(50,motorA,motorB);//70 //100
	}
}

int bleutooth_control(string *s){
	/*
		This code is for taking over the robot completely using bleutooth to make sure you can stop de robot in case its needed.
		pressing the middle button ("FIRE") on the phone stops the robot due to it not being included in the code here.
		To get in to this function you have to press "B"
	*/
	TFileIOResult nBTCmdRdErrorStatus;
 	int nSizeOfMessage;
 	ubyte nRcvBuffer[kMaxSizeOfMessage];
 	int stopcode = 0;

	while (*s != "A"){//if A is pressed the robot will resume its duty the normal way
		nSizeOfMessage = cCmdMessageGetSize(INBOX);
    if (nSizeOfMessage > kMaxSizeOfMessage){
    	nSizeOfMessage = kMaxSizeOfMessage;
 		}
 		if (nSizeOfMessage > 0){
			nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
		  nRcvBuffer[nSizeOfMessage] = '\0';

		 	*s = "";
		  stringFromChars(*s, (char *) nRcvBuffer);//put bluetooth input in string s
		  displayCenteredBigTextLine(4, *s);
		}
		//The next 4 if statements are for controlling the robot manually
		if(*s == "LEFT"){//if bleutooth input is left turn left
		  motor(motorA) = 0;
		  motor(motorB) = 30;
		  startTask(music);//make sure the music is running
		}
    else if(*s == "RIGHT"){//if bleutooth input is right turn right
    	motor(motorA) = 30;
    	motor(motorB) = 0;
      startTask(music);
    }
    else if (*s == "UP"){//if bleutooth input is up drive forward
    	setMultipleMotors(50, motorB, motorA);
    	startTask(music);
    }
    else if (*s == "DOWN"){//if bleutooth input is down drive backwards
    	setMultipleMotors(-50, motorB, motorA);
    	startTask(music);
    }
    else {//if there is no correct input turn off the motors
    	setMultipleMotors(0, motorA, motorB);
    	stopTask(music);//make sure the music is stopped
    	clearSounds();//empty the buffer
    }
    if (*s == "C"){//if the C button is pressed the loop stops and the stopcode == 1 to stop de code from running
    	stopcode = 1;
    	stopTask(music);
    	clearSounds();
    	break;
    }
  }
  *s = "";//make sure s is empty
  return stopcode;//output of the function used to stop the code if chosen
}

void junction(string *junction_string){
	/*
		This code stops the robot at an intersection to make sure you can chose for it to go straight left or right.
		In case the direction is already chosen it just goes to the already chosen direction
	*/

	if (SensorValue[S1] < 38 && SensorValue[S2] < 55){//was 50
		setMultipleMotors(50, motorA, motorB);//Drive the cart forward a little for 40 miliseconds. This way it ends up more straight on the line after turning
		wait1Msec(50);
		stopTask(music);//stops the music
		clearSounds();//clears the sound buffer
		setMultipleMotors(0, motorA, motorB);//stop the robot
		wait(0.02);

		while (1){
			if(*junction_string == "LEFT"){//if the input is "LEFT" turn left until you read the black line color and then continue your normal duty
				motor(motorA) = 0;
				motor(motorB) = 40;
				*junction_string = "";
				while(1){
					if (SensorValue[S1] < 40){//color sensor check
						setMultipleMotors(0, motorA, motorB);
						break;
					}
				}
				break;
			}

			else if(*junction_string == "RIGHT"){//the same only then for turning right
				motor(motorA) = 40;
				motor(motorB) = 0;
				*junction_string = "";
				while(1){
					if (SensorValue[S2] < 65){//light sensor check
						setMultipleMotors(0, motorA, motorB);
						break;
					}
				}
				break;
			}

			//if up just stop the loop to exit
			else if(*junction_string == "UP"){
				setMultipleMotors(50, motorA, motorB);
				wait(0.2);
				break;
			}

			else if (*junction_string == ""){//When no input stop the robot
				setMultipleMotors(0, motorA, motorB);
				check_bleutooth(junction_string);
			}
		}
	}
}


task main()
{
	/*
		The main function contains just an infinite while loop that continues to call all the functions before mentioned. In the called
		functions we check for specific things like the line curving or a bleutooth input.
	*/
	int stopcode2 = 0;//for stopping the while loop in case of a remote code shutdown

	startTask(music);//start the music in a seperate thread

	string s = "";//create a string
	TFileIOResult nBTCmdRdErrorStatus;
  int nSizeOfMessage;
  ubyte nRcvBuffer[kMaxSizeOfMessage];

	while(1){
		 nSizeOfMessage = cCmdMessageGetSize(INBOX);

    if (nSizeOfMessage > kMaxSizeOfMessage){//make the message shorter if its to long
      nSizeOfMessage = kMaxSizeOfMessage;
    }

    if (nSizeOfMessage > 0){//if the message is not empty, put the received information in string s
    	nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
    	nRcvBuffer[nSizeOfMessage] = '\0';
    	s = "";
    	stringFromChars(s, (char *) nRcvBuffer);//put the received data in string s
    	displayCenteredBigTextLine(4, s);
    }
    if (s == "B"){//bluetooth override
    	setMultipleMotors(0, motorA, motorB);
			stopcode2 = bleutooth_control(&s);//check for bleutooth input. This is the version of bleutooth input that takes over the robot
		}
		junction(&s);//check for an intersection
		linefollow();//check if the line is curving and if not then just drive
		if(stopcode2 == 1){//if "C" is pressed in the bleutooth_control function then stop the code completely
			return;
		}
	}
}
