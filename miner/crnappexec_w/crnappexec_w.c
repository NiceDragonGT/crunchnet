/*
crnappexec_w.c: CrunchNet Miner application execution program
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <WinSock2.h>

// Entry point of application
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	AllocConsole();
	// Command line variables
	int argc; // argc
	LPWSTR* argv; // argv
	argv = CommandLineToArgvW(GetCommandLine(), &argc); // Converts lpCmdLine into argc and argv

	free(argv); // Deletes command line from memory
	return 0;
}
