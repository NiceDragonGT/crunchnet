/*
crunchmin_l.c: CrunchNet Miner source (Linux)
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

// Internal CrunchNet include
#include "crnminer_l.h"

// X colour variables
XColor deeppinkC, hotpinkC, lightpinkC, greyC;
Colormap colourMap;

// Colours
char deeppink[] = "#FF1493";
char hotpink[] = "#FF69B4";
char lightpink[] = "#FFB6C1";
char grey[] = "#C0C0C0";

// Entry point of application
int main(){
    // Checks for update
	BOOL updtemp = CheckForUpdate();
	if (updtemp == 1) { // Checks if update is required
		return 0; // Terminates application
	}
	else if (updtemp == 2) { // Checks if there was an error requesting an update
		return 1; // Terminates application
	}
	else {
        // Prints startup message to console
        printf("======================================\n");
        printf("CrunchNet Miner (Linux) | Version 0.1a\n");
        printf("Copyright (c) 2018 Kade Burnside\n");
        printf("======================================\n\n");
        printf("This program is licensed under the GNU GPLv3.\n\n");

        // Window variables
        Display* display; // Display
        int screen; // Screen
        Window window; // Window
        XEvent event; // Window event

        // Open connection with X Server
        display = XOpenDisplay(NULL);
        if(!display){
            printf("Cannot open display\n");
            return 1;
        }
        screen = DefaultScreen(display);

        // Creates background colour of window
        colourMap = DefaultColormap(display, screen);
        XParseColor(display, colourMap, lightpink, &lightpinkC);
        XAllocColor(display, colourMap, &lightpinkC);

        // Creates window
        window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 600, 370, 1,
            BlackPixel(display, screen), lightpinkC.pixel);

        // Process window close event through event handler so XNextEvent does not fail
        Atom delWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
        XSetWMProtocols(display, window, &delWindow, 1);

        XSelectInput(display, window, ExposureMask | KeyPressMask); // Select types of events
        XMapWindow(display, window); // Display window
        XStoreName(display, window, "CrunchNet Miner (Inactive)"); // Sets window title

        /* Creates other colours */
        // Hot pink
        XParseColor(display, colourMap, hotpink, &hotpinkC);
        XAllocColor(display, colourMap, &hotpinkC);

        // Deep pink
        XParseColor(display, colourMap, deeppink, &deeppinkC);
        XAllocColor(display, colourMap, &deeppinkC);

        // Grey
        XParseColor(display, colourMap, grey, &greyC);
        XAllocColor(display, colourMap, &greyC);

        /* Stores text items */
        const short textNum = 13; // Number of text items
        XTextItem* text[textNum]; // Text items (XTextItem)
        // Allocates memory
        for(short i = 0; i < textNum; i++){
            text[i] = malloc(sizeof(XTextItem));
        }

        // Header text
        text[0]->chars = "CrunchNet Miner Dashboard";
        text[0]->delta = 0;
        text[0]->font = 0;
        text[0]->nchars = 25;

        // Account section header
        text[1]->chars = "Account options";
        text[1]->delta = 0;
        text[1]->font = 0;
        text[1]->nchars = 15;

        // Earnings information header
        text[2]->chars = "Earnings information";
        text[2]->delta = 0;
        text[2]->font = 0;
        text[2]->nchars = 20;

        // Miner configuration header
        text[3]->chars = "Miner configuration";
        text[3]->delta = 0;
        text[3]->font = 0;
        text[3]->nchars = 19;

        // Miner session length label
        text[4]->chars = "Miner session length:";
        text[4]->delta = 0;
        text[4]->font = 0;
        text[4]->nchars = 21;

        // Session earnings label
        text[5]->chars = "Session earnings:";
        text[5]->delta = 0;
        text[5]->font = 0;
        text[5]->nchars = 17;

        // Total balance label
        text[6]->chars = "Total balance:";
        text[6]->delta = 0;
        text[6]->font = 0;
        text[6]->nchars = 14;

        // CryptX/hour label
        text[7]->chars = "CryptX/hour:";
        text[7]->delta = 0;
        text[7]->font = 0;
        text[7]->nchars = 12;

        // CryptX/day label
        text[8]->chars = "CryptX/day:";
        text[8]->delta = 0;
        text[8]->font = 0;
        text[8]->nchars = 11;

        // CryptX/day label
        text[9]->chars = "Useful if you use multiple computers";
        text[9]->delta = 0;
        text[9]->font = 0;
        text[9]->nchars = 36;

        // Save location label
        text[10]->chars = "Save location:";
        text[10]->delta = 0;
        text[10]->font = 0;
        text[10]->nchars = 14;

        // Allocated storage label
        text[11]->chars = "Allocated storage:";
        text[11]->delta = 0;
        text[11]->font = 0;
        text[11]->nchars = 18;

        // Max processes label
        text[12]->chars = "Maximum number of processes:";
        text[12]->delta = 0;
        text[12]->font = 0;
        text[12]->nchars = 28;

        BOOL isWindowOpen = TRUE;

        // Event loop
        while(isWindowOpen){
            XNextEvent(display, &event);
            switch(event.type){
            case Expose: // Draw/redraw the window (equivalent of WM_PAINT in Windows). Also used for WM_CREATE
                // Shapes
                XSetForeground(display, DefaultGC(display, screen), deeppinkC.pixel); // Sets foreground colour to deep pink
                XFillRectangle(display, window, DefaultGC(display, screen), 0, 0, 600, 30); // Draws top rectangle
                XFillRectangle(display, window, DefaultGC(display, screen), 0, 330, 600, 40); // Draws bottom rectangle
                XSetForeground(display, DefaultGC(display, screen), hotpinkC.pixel); // Sets foreground colour to hot pink
                XFillRectangle(display, window, DefaultGC(display, screen), 10, 50, 285, 70); // Draws account options section
                XFillRectangle(display, window, DefaultGC(display, screen), 10, 140, 285, 130); // Draws earnings information section
                XFillRectangle(display, window, DefaultGC(display, screen), 305, 50, 285, 220); // Draws miner configuration section
                XFillRectangle(display, window, DefaultGC(display, screen), 10, 280, 580, 40); // Draws rectangle containing miner toggle button

                // Text
                XSetForeground(display, DefaultGC(display, screen), WhitePixel(display, screen)); // Sets text colour to white
                XDrawText(display, window, DefaultGC(display, screen), 220, 18, text[0], 1); // Draws header text
                XSetForeground(display, DefaultGC(display, screen), BlackPixel(display, screen)); // Sets text colour to black
                XDrawText(display, window, DefaultGC(display, screen), 110, 44, text[1], 1); // Draws account options header
                XDrawText(display, window, DefaultGC(display, screen), 95, 134, text[2], 1); // Draws earnings information header
                XDrawText(display, window, DefaultGC(display, screen), 395, 44, text[3], 1); // Draws miner configuration header
                XDrawText(display, window, DefaultGC(display, screen), 15, 159, text[4], 1); // Draws miner session length label
                XDrawText(display, window, DefaultGC(display, screen), 15, 179, text[5], 1); // Draws session earnings label
                XDrawText(display, window, DefaultGC(display, screen), 15, 199, text[6], 1); // Draws total balance label
                XDrawText(display, window, DefaultGC(display, screen), 15, 219, text[7], 1); // Draws CryptX/hour label
                XDrawText(display, window, DefaultGC(display, screen), 15, 239, text[8], 1); // Draws CryptX/day label
                XDrawText(display, window, DefaultGC(display, screen), 340, 89, text[9], 1); // Draws miner name description
                XDrawText(display, window, DefaultGC(display, screen), 310, 106, text[10], 1); // Draws save location label
                XDrawText(display, window, DefaultGC(display, screen), 310, 149, text[11], 1); // Draws allocated storage label
                XDrawText(display, window, DefaultGC(display, screen), 310, 169, text[12], 1); // Draws max processes label

                /* Static text controls */
                // Background
                XSetForeground(display, DefaultGC(display, screen), WhitePixel(display, screen)); // Sets foreground colour to white
                XFillRectangle(display, window, DefaultGC(display, screen), 140, 147, 150, 18); // Miner session length
                XFillRectangle(display, window, DefaultGC(display, screen), 120, 167, 170, 18); // Session earnings
                XFillRectangle(display, window, DefaultGC(display, screen), 120, 187, 170, 18); // Total balance
                XFillRectangle(display, window, DefaultGC(display, screen), 120, 207, 170, 18); // CryptX/hour
                XFillRectangle(display, window, DefaultGC(display, screen), 120, 227, 170, 18); // CryptX/day
                XFillRectangle(display, window, DefaultGC(display, screen), 310, 114, 275, 20); // Save location

                /* Text boxes */
                // Background
                XFillRectangle(display, window, DefaultGC(display, screen), 15, 55, 275, 20); // Username
                XFillRectangle(display, window, DefaultGC(display, screen), 310, 55, 275, 20); // Miner name
                XFillRectangle(display, window, DefaultGC(display, screen), 420, 137, 100, 18); // Allocated storage text

                /* Buttons */
                // Background
                XSetForeground(display, DefaultGC(display, screen), greyC.pixel); // Sets foreground colour to grey
                XFillRectangle(display, window, DefaultGC(display, screen), 15, 85, 275, 30); // Check for username
                XFillRectangle(display, window, DefaultGC(display, screen), 395, 93, 190, 18); // Browse
                XFillRectangle(display, window, DefaultGC(display, screen), 147, 285, 150, 30); // Start/stop miner
                XFillRectangle(display, window, DefaultGC(display, screen), 303, 285, 150, 30); // Save configuration
                XFillRectangle(display, window, DefaultGC(display, screen), 147, 335, 150, 30); // About CrunchNet
                XFillRectangle(display, window, DefaultGC(display, screen), 303, 335, 150, 30); // View license

                /* Drop-down lists */

                /* Trackbars */
                // Background
                XSetForeground(display, DefaultGC(display, screen), WhitePixel(display, screen)); // Sets foreground colour to grey
                XFillRectangle(display, window, DefaultGC(display, screen), 310, 179, 275, 40); // Max processes

                break;
            case ClientMessage: // Handle window close event (equivalent of WM_DESTROY in Windows)
                WriteMessage("Dashboard window closed", 0);
                isWindowOpen = FALSE;
                break;
            }
        }

        XDestroyWindow(display, window); // Destroys window
        XCloseDisplay(display); // Closes connection to X Server

        // Frees memory containing text items
        for(short i = 0; i < textNum; i++){
            free(text[i]);
        }

        return 0;
    }
}
