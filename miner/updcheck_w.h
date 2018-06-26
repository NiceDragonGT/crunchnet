/*
updcheck_w.h: CrunchNet Updater check function
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

#ifndef UPDCHECK_W_H
#define UPDCHECK_W_H

// Windows API include
#include <WinSock2.h> // Includes Windows.h

// Sets defenition of NULL to 0
#undef NULL
#define NULL 0

// Update check function
BOOL CheckForUpdate(WSADATA* wsa){
	return FALSE;
}

#endif
