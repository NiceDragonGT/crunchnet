/*
crnminer_l.c: CrunchNet Miner console source (Linux)
Copyright (c) 2018 Kade Burnside
*/

/*
This file is part of CrunchNet.

CrunchNet is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CrunchNet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CrunchNet. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CRNMINER_L_H
#define CRNMINER_L_H

// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// X11 include
#include <X11/Xlib.h>

// Internal CrunchNet include
#include <miner/updcheck_l.h>

BOOL isMinerRunning = FALSE; // Miner status
BOOL promptStatus = FALSE; // Miner termination prompt status

time_t systime_raw;
struct tm* systime;

FILE* fiop; // File IO pointer

// Miner configuration
LARGE_INTEGER accountId, storageAlloc; // Account ID and storage space to be allocated (in bytes)
unsigned short maxProcesses; // CrunchNet applications that can run at once (limit is ???)
const char* accountName; // Username
const char* minerName; // Miner name
const char* saveLocation; // Save location

// Time macros
#define HOUR systime->tm_hour
#define MIN systime->tm_min
#define SEC systime->tm_sec

// Gets data from configuration file
void GetConfig(){

}

// Saves data to configuration file
void SaveConfig(){

}

// Prints time to console
void WriteTime(int destination){
    char timestr[11]; // Time string
    // Gets system time
    time(&systime_raw);
    systime = gmtime(&systime_raw);
    // Converts time to string and prints to console
    if (HOUR < 10) if (MIN < 10) if (SEC < 10) sprintf(timestr, "[0%d:0%d:0%d]", HOUR, MIN, SEC);
	else sprintf(timestr, "[0%d:0%d:%d]", HOUR, MIN, SEC);
	else if (SEC < 10) sprintf(timestr, "[0%d:%d:0%d]", HOUR, MIN, SEC);
	else sprintf(timestr, "[0%d:%d:%d]", HOUR, MIN, SEC);
	else if (MIN < 10) if (SEC < 10) sprintf(timestr, "[%d:0%d:0%d]", HOUR, MIN, SEC);
	else sprintf(timestr, "[%d:0%d:%d]", HOUR, MIN, SEC);
	else if (SEC < 10) sprintf(timestr, "[%d:%d:0%d]", HOUR, MIN, SEC);
	else sprintf(timestr, "[%d:%d:%d]", HOUR, MIN, SEC);

	if (destination == NULL) printf("%s ", timestr); // Prints time string to console
	else fprintf(fiop, "%s ", timestr); // Saves time string to log file
}

// Prints message to console
void WriteMessage(const char* message, int destination){
    if(destination == NULL){
        WriteTime(NULL); // Prints time
        printf("%s\n", message); // Prints message and line break to console
    }
    else if(destination == 1){
        WriteTime(1); // Saves time to log file
        fprintf(fiop, "%s\n", message); // Saves message to log file
    }
    else{
        WriteTime(NULL); // Prints time
        printf("%s\n", message); // Prints message
        WriteTime(1); // Saves time to log file
        fprintf(fiop, "%s\n", message); // Saves message to log file
    }
}

// Makes 2 errors (one on the console/in log file, one as a message box)
void WriteError(const char* message){
    WriteMessage(message, 2); // Prints error
    // Message box implementation not available yet
}

// Resets miner if error occurs
void ResetMiner(Display* d, Window w){
    isMinerRunning = FALSE; // Sets miner status to inactive
	// Saves end message to log file
	fprintf(fiop, "Log ended at %d/%d/%d ", systime->tm_mday, systime->tm_mon, systime->tm_year);
	WriteTime(1); // Saves current time to log file
	fprintf(fiop, "\n\n"); // Saves line break character to log file
	fclose(fiop); // Closes log file stream
	XStoreName(d, w, "CrunchNet Miner (Inactive)"); // Changes text of dashboard window
	// Button implementation not available yet
}

// Updates timer every second
void* UpdateTimer(){
    char time[9]; // Time string
	// Time units
	short hour = 0;
    short min = 0;
	short sec = 0;
	// Loop until miner has stopped
	while (isMinerRunning) {
		usleep(1000000); // Waits 1 second
		// Appends current time
		sec++; if (sec == 60) {
			sec = 0; min++; if (min == 60) {
				min = 0; hour++;
			}
		}
		// Converts time to string
		if (hour < 10) if (min < 10) if (sec < 10) sprintf(time, "0%d:0%d:0%d", hour, min, sec);
		else sprintf(time, "0%d:0%d:%d", hour, min, sec);
		else if (sec < 10) sprintf(time, "0%d:%d:0%d", hour, min, sec);
		else sprintf(time, "0%d:%d:%d", hour, min, sec);
		else if (min < 10) if (sec < 10) sprintf(time, "%d:0%d:0%d", hour, min, sec);
		else sprintf(time, "%d:0%d:%d", hour, min, sec);
		else if (sec < 10) sprintf(time, "%d:%d:0%d", hour, min, sec);
		else sprintf(time, "%d:%d:%d", hour, min, sec);
		// Static text implementation not available yet
	}
	// Static text implementation not available yet
	return 0;
}

// Miner execution function
void* MinerExecution(void* unused) {
	// Miner execution variables
	unsigned short currentProcesses = 0; // Number of processes currently running

	// Miner execution loop
	while (isMinerRunning) {
		// Checks if miner termination prompt has been sent
		if (!promptStatus) {
			// If not, check if all processes are being ultilized
			if (currentProcesses < maxProcesses) {
				// If not, find application to run
			}
		}
		else {
			// Prints termination message to console
			WriteMessage("Miner termination prompt sent. Preparing to terminate miner.", 2);
			while(currentProcesses) break; // Waits for all processes to finish

			/* Notifies CrunchNet that miner has ended */

			isMinerRunning = FALSE; // Sets miner status to false
		}
	}
	return 0;
}

// Starts miner
void StartMiner(Display* d, Window w){
    isMinerRunning = TRUE; // Sets miner status to active
	XStoreName(d, w,"CrunchNet Miner (Active)"); // Changes text of dashboard window
	// Button implementation not available yet
	pthread_t timerThread, minerThread; // Threads
	pthread_create(&timerThread, NULL, UpdateTimer(), NULL); // Starts timer

	/* Miner initialization */
	// Gets current time
	time(&systime_raw);
    systime = gmtime(&systime_raw);

    // Opens log file
    fiop = fopen("log.txt", "a");
	if(!fiop){
		// Saves start message to log file
		fprintf(fiop, "Log started at %d/%d/%d ", systime->tm_mday, systime->tm_mon, systime->tm_year);
		WriteTime(1);
		fprintf(fiop, "\n"); // Prints line break character
	}
	else{
		WriteError("Could not open log file."); // Prints error
		ResetMiner(d, w);
		return;
	}

	// Checks for Internet connection
	if(!( /* Requires function for checking status of Internet connection */ 0)){
		WriteError("There is no Internet connection. Reconnect and try again."); // Prints error
		ResetMiner(d, w); // Resets miner
		return;
	}

	/* Notifies CrunchNet that miner has started */

	WriteMessage("Miner initialization succeeded. Now mining", 2); // Prints initialization success message
	pthread_create(&minerThread, NULL, MinerExecution, NULL); // Starts miner thread
}

// Stops miner
void StopMiner(Display* d, Window w){
    promptStatus = FALSE; // Sends miner loop termination prompt
	while (isMinerRunning) break; // Waits for miner to finish
	WriteMessage("Miner has stopped.", 2); // Prints miner termination message to console and log file
	ResetMiner(d, w); // Resets miner
}

#endif
