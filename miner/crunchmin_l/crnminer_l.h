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

// Internal CrunchNet include
#include <miner/updcheck_l.h>

BOOL isMinerRunning = FALSE; // Miner status

FILE* fiop; // File IO pointer

// Gets data from configuration file
void GetConfig(){

}

// Saves data to configuration file
void SaveConfig(){

}

// Prints time to console
void WriteTime(int destination){

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

}

// Resets miner if error occurs
void ResetMiner(){

}

// Updates timer every second
void UpdateTimer(){

}

// Starts miner
void StartMiner(){

}

// Stops miner
void StopMiner(){

}

#endif
