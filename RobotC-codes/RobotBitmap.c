
#pragma config(Sensor, S1,     color_sensor,   sensorColorNxtRED)
#pragma config(Sensor, S2,     light_sensor,   sensorLightActive)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,          motor_right,   tmotorNXT, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          motor_left,    tmotorNXT, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(NXT)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//long nLastXmitTimeStamp = nPgmTime;
//long nDeltaTime = 0;

//const int kMaxSizeOfMessage = 30; //create a variable for the maximum message size
//const int INBOX = 5;

//variables for setting the minimum and maximum values of the light sensor and colorsensor
int maxcolor = 56;
int mincolor = 23;
int maxlight = 72;
int minlight = 38;
int counter = 0;
//int i,j;
int holding1,holding2;
int startrow, startcollumn;
int endrow, endcollumn;
int currentrow,currentcollumn;
int currentjunctionrow = 0,currentjunctioncollumn = 0;
char first_junction = '0';
char new_matrix[20];
char turn_left = 'L';
char turn_right = 'R';
char up = 'U';
char down = 'D';
void steering_sideways();
void finding_route();

int matrix[7][9] =				{     {0,1,0,1,0,1,0,1,0},
                                {1,4,1,4,1,4,1,4,1},
                                {0,1,0,1,0,1,0,1,0},
                                {1,4,1,4,1,5,1,4,1},
                                {0,1,0,1,0,1,0,1,0},
                                {1,4,1,4,1,4,1,4,1},
                                {0,2,0,1,0,1,0,1,0}};

int junctions[20][2];

char instructions[20];

void speed_adjust(int index, int until, int increment) {
    /*
            This function is used for slowing down the motor slowly instead of stopping immediatly
     */
    for (int i = index; i >= until; i += increment) {
        setMultipleMotors(i, motorA, motorB);
        wait(0.09);
    }
}

void left(){
        turn_left = 'U';
        turn_right = 'D';
        up = 'R';
        down = 'L';
}
void right(){
    turn_right = 'U';
    turn_left = 'D';
    up = 'L';
    down = 'R';
}

void up_settings(){
    turn_right = 'R';
    turn_left = 'L';
    up = 'U';
    down = 'D';
}

void adding_instructions(char first_junction){
    instructions[counter] = first_junction;
    counter++;
}

void calculate(){
    int i = 0;
    nxtDisplayTextLine(1, "In function!");
    while(instructions[i] != '0'){
        	if(instructions[i] == 'l'){
            	new_matrix[i] = turn_left;
            	left();
            	i++;
            	while(1){
            		nxtDisplayTextLine(2, "Turning Left");
            			if((instructions[i] != 'u') && (instructions[i] != '0')){
                			if(instructions[i] == 'l'){
                    			new_matrix[i] = turn_left;
                			}else if(instructions[i] == 'r'){
                    			new_matrix[i] = turn_right;
                			}else if(instructions[i] == 'd'){
                    			new_matrix[i] = down;
                			}else if(instructions[i] == '0'){
                          break;
                        	}
                i++;
            		}else{
            			nxtDisplayTextLine(2, "Going up agian!");
                			new_matrix[i] = up;
                			i++;
                			up_settings();
                			break;
            }}
        }else if((instructions[i] == 'r') && (instructions[i] != '0')){
            new_matrix[i] = turn_right;
            right();
            i++;
            while(1){
            	nxtDisplayTextLine(3, "Turning Right");
                if(instructions[i] != 'u'){
                        if(instructions[i] == 'l'){
                            new_matrix[i] = turn_left;
                        }else if(instructions[i] == 'r'){
                            new_matrix[i] = turn_right;
                        }else if(instructions[i] == 'd'){
                            new_matrix[i] = down;
                        }else if(instructions[i] == '0'){
                            break;
                        }
                i++;
                }else if(instructions[i] == 'u'){
                    new_matrix[i] = up;
                    i++;
                    up_settings();
                    break;
                }
            }
        }else if((instructions[i] == 'u') && (instructions[i] != '0')){
          nxtDisplayTextLine(4, "Going up");
        	new_matrix[i] = up;
            i++;
        }
    }
}


void steering_sideways(){
    if(currentcollumn < endcollumn){ //++
			if(matrix[currentrow][currentcollumn+1] == 1){
				currentcollumn++;
                adding_instructions('r');
			}else if(matrix[currentrow][currentcollumn+1] == 4){
                currentcollumn++;
                currentjunctionrow = currentrow;
                currentjunctioncollumn = currentcollumn;
                junctions[counter][0] = currentjunctionrow;
                junctions[counter][1] = currentjunctioncollumn;
                if(currentrow == endrow){
				if(matrix[currentrow][currentcollumn+1] == 1){
					currentcollumn++;
                    adding_instructions('r');
                }}else{ //++
                    finding_route();
                    adding_instructions('u');
                }
		}else if(matrix[currentrow][currentcollumn+1] == 5){
            currentcollumn++;
            adding_instructions('0');
        }
	}else if(currentcollumn > endcollumn){ //--
				if(matrix[currentrow][currentcollumn-1] == 1){
						currentcollumn--;
			}else if(matrix[currentrow][currentcollumn-1] == 4){
                currentcollumn--;
                adding_instructions('l');
                currentjunctionrow = currentrow;
                currentjunctioncollumn = currentcollumn;
                junctions[counter][0] = currentjunctionrow;
                junctions[counter][1] = currentjunctioncollumn;
                if(currentrow == endrow){
									if(matrix[currentrow][currentcollumn-1] == 1){
										currentcollumn--;
                    adding_instructions('l');
                }}else{
                    finding_route();
                    adding_instructions('u');
                }
    }else if(matrix[currentrow][currentcollumn-1] == 5){
            currentcollumn--;
            adding_instructions('0');
    }
    }}



void finding_place(int place_number)
{
		for(int i = 0; i<7; i++){
    	for (int k = 0; k<9; k++){
    		if (matrix[i][k] ==place_number){
    			holding1 = i;
    			holding2 = k;
				}
			}
  	}
}

void get_places(){
    finding_place(2);
    startrow = holding1;
    startcollumn = holding2;
    finding_place(5);
    endrow = holding1;
    endcollumn = holding2;
    currentrow = startrow;
    currentcollumn = startcollumn;

}

void finding_route(){
	if(currentrow < endrow){ //++
		if(matrix[currentrow + 1][currentcollumn] == 1){
			currentcollumn++;
		}else if(matrix[currentrow+1][currentcollumn] == 4){
			currentrow++;
            currentjunctionrow = currentrow;
            currentjunctioncollumn = currentcollumn;
            junctions[counter][0] = currentjunctionrow;
            junctions[counter][1] = currentjunctioncollumn;
			if(currentrow != endrow){
			if(matrix[currentrow+1][currentcollumn] == 1){
				currentrow++;
                adding_instructions('u');
			}else{
                steering_sideways();
			}}
			else{
                steering_sideways();
				//sideways
			}
		}else if(matrix[currentrow+1][currentcollumn] == 5){
			currentrow++;
            adding_instructions('\0');
		}else if(matrix[currentrow+1][currentcollumn] == 0){
            steering_sideways();
        }
	}else if(currentrow > endrow){ //--
		if(matrix[currentrow - 1][currentcollumn] == 1){
			currentrow--;
		}else if(matrix[currentrow- 1][currentcollumn] == 4){
			currentrow--;
            currentjunctionrow = currentrow;
            currentjunctioncollumn = currentcollumn;
            junctions[counter][0] = currentjunctionrow;
            junctions[counter][1] = currentjunctioncollumn;
			if(currentrow != endrow){
			if(matrix[currentrow -1][currentcollumn] == 1){
				currentrow--;
                adding_instructions('u');
			}else{
                steering_sideways();
			}}
			else{
                steering_sideways();
				//sideways
			}
		}else if(matrix[currentrow-1][currentcollumn] == 5){
			currentrow--;
            adding_instructions('\0');
		}else if(matrix[currentrow-1][currentcollumn] == 0){
            steering_sideways();
        }
	}else if(currentrow == endrow){ //sideways
		steering_sideways();
}
}






task music() {
    //plays music while robot is in motion
    playSoundFile("Supermario.rmd");
    wait1Msec(10000);
}

void linefollow(void) {
    //using float variable for the sensors to have the most accurate
    float color;
    float light;
    float sonar;

    // reading the sensor value
    light = SensorValue[S2];
    color = SensorValue[S1];
    sonar = SensorValue[S3];

    // displaying the values from above
    //nxtDisplayTextLine(1, "color: %d", color);
    //nxtDisplayTextLine(2, "light: %d", light);
    //nxtDisplayTextLine(3, "sonar: %d", sonar);

    // if an object is closer then 30 centimeters do the folowing
    if (sonar < 30) {
        speed_adjust(60, 0, -4); //slow down

        for (int i = 0; i < 10;) {//slowly start turning
            motor[motorA] = i;
            motor[motorB] = -i;
            wait(0.4);
            i++;
        }

        while (1) {//wait until you see the line
            if (SensorValue[S2] < 60) {
                for (int i = 10; i < 0;) {//slow down the turning speed
                    motor[motorA] = i;
                    motor[motorB] = -i;
                    wait(0.4);
                    i += 2;
                }
                break;
            }
        }
    }        //redirect the robot by using the light sensor
    else if (light < (maxlight - 5)) {
        // float variable for the formule math
        float formule;
        float omrek;
        float formule2;
        //omrek is used to can use the formules in every surroundings

        omrek = light - minlight;
        // show the result of the omrek on line 4
        nxtDisplayTextLine(4, "lomrek : %f", omrek);
        // if omrek is lower then 5 use 100 and 1 for a real sharp turn
        if (omrek < 5) {
            formule = 100;
            formule2 = 1;
        }// if the omrek is between 5 and 10 use a formule that gives a heavier control
        else if (omrek < 10) {
            formule = -0.4 * omrek + 100;
            formule2 = -1.428 * omrek + 50;
        }// the standaard formule for following the line
        else {
            formule = -0.9 * omrek + 100;
            formule2 = -1.428 * omrek + 50;
        }

        startTask(music); //starts the music
        // give the motor a and b the result of the formules
        motor(motorA) = formule; //50+((60-light)*2.5)
        motor(motorB) = formule2; //35  //0
    }        // redirecting the robot by using hte color sensor
    else if (color < (maxcolor - 5)) {//vorige waarde: 4
        // float variables for best accurate values
        float formule;
        float omrek;
        float formule2;
        //omrek is used to can use the formules in every surroundings
        omrek = color - mincolor;
        // show the result of the omrek on line 5
        nxtDisplayTextLine(5, "comrek : %d", omrek);
        // if omrek is lower then 5 use 100 and 1 for a real sharp turn
        if (omrek < 5) {
            formule = 100;
            formule2 = 1;
        }// if the omrek is between 5 and 10 use a formule that gives a heavier control
        else if (omrek < 10) {
            formule = -0.4 * omrek + 100;
            formule2 = -1.428 * omrek + 50;
        }// the standaard formule for following the line
        else {
            formule = -0.9 * omrek + 100;
            formule2 = -1.428 * omrek + 50;
        }

        startTask(music); //starts the music
        // give the motor a and b the result of the formules
        motor(motorA) = formule2; //0
        motor(motorB) = formule; //50+(50-color)*1+(2/3))
    } else {
        //if their isn't steering nessasary then both motors gets 50 as speed
        setMultipleMotors(10, motorA, motorB); //70 //100
    }
}

void junction() {
    /*
            This code stops the robot at an intersection to make sure you can chose for it to go straight left or right.
            In case the direction is already chosen it just goes to the already chosen direction
     */

    if (SensorValue[S1] < (mincolor + 10) && SensorValue[S2] < (minlight + 10)) {//the minimul light value + 10 for the correct moment to stop at a line
    		setMultipleMotors(50, motorA, motorB); //Drive the cart forward a little for 50 miliseconds. This way it ends up more straight on the line after turning
        wait1Msec(50);
        stopTask(music); //stops the music
        clearSounds(); //clears the sound buffer
        setMultipleMotors(0, motorA, motorB); //stop the robot
        wait(1);

        while (1) {
        		nxtDisplayTextLine(0, "Counter: %d %c", counter, new_matrix[counter]);
            if (new_matrix[counter] == 'L') {//if the input is "LEFT" turn left until you read the black line color and then continue your normal duty
                motor(motorA) = 0;
                motor(motorB) = 40;
                while (1) {
                    if (SensorValue[S1] < 30) {//color sensor check
                        wait1Msec(5);
                        setMultipleMotors(0, motorA, motorB);
                        counter++;
                        break;
                    }
                }
                break;
            } else if (new_matrix[counter] == 'R') {//the same only then for turning right
                motor(motorA) = 40;
                motor(motorB) = 0;
                while (1) {
                    if (SensorValue[S2] < 50) {//light sensor check
                        wait1Msec(5);
                        setMultipleMotors(0, motorA, motorB);
                        counter++;
                        break;
                    }
                }
                break;
            }                //if up just stop the loop to exit
            else if (new_matrix[counter] == 'U') {
                setMultipleMotors(50, motorA, motorB);
                wait(0.2);
                counter++;
                break;
            }
        }
    }
}

task main() {
    /*
            The main function contains just an infinite while loop that continues to call all the functions before mentioned. In the called
            functions we check for specific things like the line curving or a bleutooth input.
     */
    nxtDisplayTextLine(0, "Start");
    get_places();
    nxtDisplayTextLine(1, "Map calibrated!");
    nxtDisplayTextLine(7, "[%d][%d][%d][%d]",startrow,startcollumn,endrow,endcollumn);
    while(matrix[currentrow][currentcollumn] != matrix[endrow][endcollumn]){
      nxtDisplayTextLine(3, "Finding route");
    	finding_route();
    }
    nxtDisplayTextLine(4, "Calculating directions!");
    nxtDisplayTextLine(6, "%c %c %c ",instructions[0], instructions[1],instructions[2]);
    calculate();
    nxtDisplayTextLine(5, "Route Found!");
    nxtDisplayTextLine(2, "%c,%c,%c", new_matrix[0], new_matrix[1], new_matrix[2]);
    int stopcode2 = 0; //for stopping the while loop in case of a remote code shutdown

    startTask(music); //start the music in a seperate thread

    counter = 0;

    while (1) {
        junction(); //check for an intersection
        linefollow(); //check if the line is curving and if not then just drive
        if (stopcode2 == 1) {//if "C" is pressed in the bleutooth_control function then stop the code completely
            return;
        }
    }
}