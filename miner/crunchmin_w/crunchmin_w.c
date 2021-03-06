/*
crunchmin_w.c: CrunchNet Miner source
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

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "shell32")
#pragma comment(lib, "comdlg32")
#pragma comment(lib, "kernel32")
#pragma comment(lib, "ole32")
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet")

// Internal CrunchNet include
#include "crunchmin_w.h"

// Forward declaration of WndProc function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point of application
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	si.cb = sizeof(STARTUPINFO); // Assigns startup info structure size to si.cb
	// Checks for update
	BOOL updtemp = CheckForUpdate();
	if (updtemp == 1) { // Checks if update is required
		MessageBox(NULL, L"An update is required", L"CrunchNet Updater", NULL); // Shows message box
		// Starts CrunchNet Updater
		StartApplication(L"crnupd.exe");
		return 0; // Terminates application
	}
	else if (updtemp == 2) { // Checks if there was an error requesting an update
		return 1; // Terminates application
	}
	else {
		WNDCLASS wc = { 0 }; // Window structure

		/*
		Window structure attributes

		style: Window style
		lpfnWndProc: Pointer to WndProc function
		hInstance: Application instance
		hIcon: Icon to be used
		hCursor: Cursor to be used
		hbrBackground: Window background
		lpszClassName: Name of window structure
		*/

		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInst;
		wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(APPICON));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = CreateSolidBrush(RGB(255, 182, 193));
		wc.lpszClassName = L"crunchmin_w";

		// Registers window structure
		if (!RegisterClass(&wc)) {
			// If failed, show error message box
			MessageBox(NULL, L"Window structure registration failed!", szTitle, MB_ICONERROR);
			return 1;
		}

		ghInst = hInst; // Stores hInst in global variable

		GetConfig(); // Gets data from configuration file

		AllocConsole(); // Creates console
		SetConsoleTitle(L"CrunchNet Miner Console"); // Sets title
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Gets output handle to console

		// Prints startup message to console
		WriteConsole(hConsole, L"========================================\n", 41, &charCount, NULL);
		WriteConsole(hConsole, L"CrunchNet Miner (Windows) | Version 0.1a\n", 41, &charCount, NULL);
		WriteConsole(hConsole, L"Copyright (c) 2018 Kade Burnside\n", 33, &charCount, NULL);
		WriteConsole(hConsole, L"========================================\n\n", 42, &charCount, NULL);
		WriteConsole(hConsole, L"This program is licensed under the GNU GPLv3.\n\n", 47, &charCount, NULL);

		// Creates window
		HWND hWnd = CreateWindow(L"crunchmin_w", L"CrunchNet Miner (Inactive)",
			WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
			W_WIDTH + 20, W_HEIGHT + 39, NULL, NULL, hInst, NULL);

		// Checks if window was created successfully
		if (!hWnd) {
			// If not, show error message box
			MessageBox(NULL, L"Window creation failed!", szTitle, MB_ICONERROR);
			return 1;
		}

		ShowWindow(hWnd, nCmdShow); // Displays window
		UpdateWindow(hWnd); // Updates window

		MSG msg; // Message handle
		// Message loop
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return (int)msg.wParam;
	}
}

// Event defenitions
#define CHECK_USERNAME 1
#define BROWSE_FOLDER 2
#define START_STOP_MINER 3
#define SAVE_CONFIG 4
#define ABOUT_DISPLAY 5
#define LICENSE_DISPLAY 6

// Child window handles
HWND usernameIn, sessionLength, sessionEarningsDisplay, balanceDisplay, hourEarningsDisplay, dayEarningsDisplay,
minerNameIn, saveLocationDisplay, allocatedStorageIn, allocatedStorageList, maxProcessesTrack, minerToggle;

// Window procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE: // Creates child windows
		GetSystemInfo(&sysinfo); // Gets system info

		// Account and wallet options windows
		usernameIn = CreateTextBox(NULL, WW_CENTER / 2 + 2, 67, 250, 20, hWnd); // Username text box
		SendMessage(usernameIn, EM_SETCUEBANNER, NULL, (LPARAM)&L"Username"); // Sets placeholder text of username text box
		CreateButton(L"Check for valid username", WW_CENTER / 2 + 2, 100, 250, 30, hWnd, (HMENU)CHECK_USERNAME); // Sign out

		// Earnings information windows
		sessionLength = CreateControl(WC_STATIC, L"00:00:00", DEFCONTROL | SS_CENTER, 220, 155, 120, 20, hWnd, NULL); // Miner session length
		sessionEarningsDisplay = CreateStaticText(L"0.000000000 CYX", 210, 180, 145, 20, hWnd); // Session earnings
		balanceDisplay = CreateStaticText(L"0.000000000 CYX", 210, 205, 145, 20, hWnd); // Total balance
		hourEarningsDisplay = CreateStaticText(L"0.000000000 CYX", 210, 230, 145, 20, hWnd); // CryptX/hour
		dayEarningsDisplay = CreateStaticText(L"0.000000000 CYX", 210, 255, 145, 20, hWnd); // CryptX/day

		// Miner configuration controls
		if (minerName != 0) {
			minerNameIn = CreateTextBox(minerName, WW_CENTER + 2 + (WW_CENTER + 2) / 2 - 3, 65, 265, 20, hWnd); // Miner name
		}
		else {
			minerNameIn = CreateTextBox(NULL, WW_CENTER + 2 + (WW_CENTER + 2) / 2 - 3, 65, 265, 20, hWnd); // Miner name
		}
		SendMessage(minerNameIn, EM_SETCUEBANNER, NULL, (LPARAM)&L"Miner name"); // Adds placeholder text to miner name text box
		CreateButton(L"Browse", WW_CENTER + 190, 110, 170, 20, hWnd, (HMENU)BROWSE_FOLDER); // Browse
		if (saveLocation != 0) {
			saveLocationDisplay = CreateStaticText(saveLocation, WW_CENTER + 2 + (WW_CENTER + 2) / 2 - 3, 132, 260, 20, hWnd); // Save location display
		}
		else {
			saveLocationDisplay = CreateStaticText(NULL, WW_CENTER + 2 + (WW_CENTER + 2) / 2 - 3, 132, 260, 20, hWnd); // Save location display
		}
		allocatedStorageIn = CreateControl(WC_EDIT, L"999", DEFCONTROL | ES_CENTER | ES_NUMBER, WW_CENTER + 2 + 175, 157, 90, 20, hWnd, NULL); // Allocated storage text box
		allocatedStorageList = CreateDropDownList(WW_CENTER + 2 + 250, 155, 50, 20, hWnd); // Allocated storage unit drop down list
		SendMessage(allocatedStorageList, CB_ADDSTRING, NULL, (LPARAM)&L"MB"); // Adds MB option to unit dropdown
		SendMessage(allocatedStorageList, CB_ADDSTRING, NULL, (LPARAM)&L"GB"); // Adds GB option to unit dropdown
		SendMessage(allocatedStorageList, CB_ADDSTRING, NULL, (LPARAM)&L"TB"); // Adds TB option to unit dropdown
		SendMessage(allocatedStorageList, CB_SETCURSEL, 0, NULL); // Selects default option
		maxProcessesTrack = CreateTrackbar(WW_CENTER + 2 + (WW_CENTER + 2) / 2 - 3, 207, 265, 30, hWnd, 1, sysinfo.dwNumberOfProcessors); // Max processes trackbar
		if (maxProcesses != 0) {
			SendMessage(maxProcessesTrack, TBM_SETPOS, TRUE, (LPARAM)maxProcesses); // Sets trackbar to max processes value from configuration file
		}
		else {
			SendMessage(maxProcessesTrack, TBM_SETPOS, TRUE, (LPARAM)sysinfo.dwNumberOfProcessors); // Sets trackbar to number of threads
		}

		// Miner controls
		minerToggle = CreateButton(L"Start miner", WW_CENTER + 2 - 78, 300, 150, 30, hWnd, (HMENU)START_STOP_MINER); // Start/Stop miner
		CreateButton(L"Save configuration", WW_CENTER + 2 + 78, 300, 150, 30, hWnd, (HMENU)SAVE_CONFIG); // Save configuration

		// Window footer controls
		CreateButton(L"About CrunchNet", WW_CENTER + 2 - 78, 350, 150, 30, hWnd, (HMENU)ABOUT_DISPLAY); // About CrunchNet
		CreateButton(L"View license", WW_CENTER + 2 + 78, 350, 150, 30, hWnd, (HMENU)LICENSE_DISPLAY); // View license

		break;
	case WM_COMMAND: // Handles messages from child windows
		switch (wParam) {
		case CHECK_USERNAME:
			break;
		case BROWSE_FOLDER:
			// Checks if miner is active, and if active, show error message
			if (isMinerRunning) {
				MessageBox(hWnd, L"Cannot edit configuration while miner is active!", szTitle, MB_ICONERROR | MB_APPLMODAL);
			}
			else {
				BROWSEINFO bi = { 0 }; // Browse info structure
				LPITEMIDLIST pIdl; // Item ID list

				// Shows dialog
				bi.ulFlags = BIF_USENEWUI;
				OleInitialize(NULL);
				pIdl = SHBrowseForFolder(&bi);

				// Gets path from dialog
				if (pIdl != NULL) {
					free(saveLocation); // Deletes memory
					saveLocation = malloc(_MAX_PATH); // Re-allocates memory
					if (SHGetPathFromIDList(pIdl, saveLocation) == 0) {
						MessageBox(hWnd, L"Invalid location!", szTitle, MB_ICONERROR | MB_APPLMODAL);
					}
					CoTaskMemFree(pIdl);
				}
				OleUninitialize();
				SetWindowText(saveLocationDisplay, saveLocation); // Sets text of saveLocationDisplay to save location
			}
			break;
		case START_STOP_MINER:
			// Checks if miner is running
			if (!isMinerRunning) StartMiner(hWnd, minerToggle, sessionLength); // If miner is inactive, start miner
			else StopMiner(hWnd, minerToggle); // If miner is active, stop miner
			break;
		case SAVE_CONFIG:
			SaveConfig(usernameIn, minerNameIn, allocatedStorageIn, allocatedStorageList, maxProcessesTrack);
			break;
		case ABOUT_DISPLAY:
			// Displays information message box
			MessageBox(hWnd, L"CrunchNet Miner (Windows)\nVersion 0.1\nCopyright (c) 2018 Kade Burnside\n\nThis program is licensed under the GNU GPLv3.\nSource code is available at https://github.com/cadellapowell61/crunchnet \n\nTo get in touch with me, email me at cadellapowell61@gmail.com", L"About CrunchNet", MB_APPLMODAL);
			break;
		case LICENSE_DISPLAY:
			// Opens HTML file containing license
			ShellExecute(NULL, NULL, L"license.html", NULL, NULL, NULL);
			break;
		}
		break;
	case WM_PAINT: // Draws GDI elements on window
		// Creates brushes
		HeaderBrush = CreateSolidBrush(RGB(255, 20, 147));
		SectionBrush = CreateSolidBrush(RGB(255, 105, 180));

		// Loads window header font
		hFont = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, L"Sans");

		hdc = BeginPaint(hWnd, &ps);
		
		SetBkMode(hdc, TRANSPARENT); // Makes text background transparent

		// Shapes
		SelectObject(hdc, HeaderBrush);
		SelectObject(hdc, CreatePen(NULL, 1, RGB(255, 20, 147)));
		Rectangle(hdc, 0, 0, W_WIDTH + 4, 30); // Rectangle at top of window
		Rectangle(hdc, 0, 330, W_WIDTH + 4, 370); // Rectangle at bottom of window
		SelectObject(hdc, SectionBrush);
		SelectObject(hdc, CreatePen(NULL, 1, RGB(255, 105, 180)));
		Rectangle(hdc, 10, 50, WW_CENTER - 3, 120); // Account options section
		Rectangle(hdc, 10, 140, WW_CENTER - 3, 270); // Earnings information section
		Rectangle(hdc, WW_CENTER + 7, 50, W_WIDTH - 6, 270); // Miner configuration section
		Rectangle(hdc, 10, 280, W_WIDTH - 6, 320); // Rectangle containing miner toggle button

		// Text
		SetTextAlign(hdc, TA_CENTER); // Sets text align to center
		
		DisplayText(hdc, (WW_CENTER + 2) / 2, 32, L"Account options", 15); // Account options header
		DisplayText(hdc, (WW_CENTER + 2) / 2, 122, L"Earnings information", 20); // Earnings information header
		DisplayText(hdc, WW_CENTER + 2 + (WW_CENTER + 2) / 2, 32, L"Miner configuration", 19); // Miner configuration header

		SetTextColor(hdc, 0); // Sets text colour to black

		// Earnings information text
		SetTextAlign(hdc, TA_LEFT); // Sets text align to left
		DisplayText(hdc, 15, 147, L"Miner session length:", 21); // Miner session length
		DisplayText(hdc, 15, 172, L"Session earnings:", 19); // Session earnings
		DisplayText(hdc, 15, 197, L"Total balance:", 14); // Total balance
		DisplayText(hdc, 15, 222, L"CryptX/hour:", 12); // CryptX/hour
		DisplayText(hdc, 15, 247, L"CryptX/day:", 11); // CryptX/day

		SetTextAlign(hdc, TA_CENTER); // Sets text align to center

		// Miner configuration text
		DisplayText(hdc, WW_CENTER + 2 + (WW_CENTER + 2) / 2, 82, L"Useful if you use multiple computers", 36); // Useful if you use multiple computers
		SetTextAlign(hdc, TA_LEFT); // Sets text align to left
		DisplayText(hdc, WW_CENTER + 2 + 10, 102, L"Save location:", 14); // Save location
		DisplayText(hdc, WW_CENTER + 2 + 10, 147, L"Allocated storage:", 18); // Allocated storage
		DisplayText(hdc, WW_CENTER + 2 + 10, 172, L"Maximum number of processes:", 28); // Maximum number of processes

		// Header text
		SelectObject(hdc, hFont);
		SetTextColor(hdc, 0xffffff);
		SetTextAlign(hdc, TA_CENTER); // Sets text align to center
		DisplayText(hdc, WW_CENTER + 2, 3, L"CrunchNet Miner Dashboard", 25);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: // Destroys window
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
