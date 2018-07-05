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
		StartApplication(L"crnupd_w.exe");
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

		// Opens configuration file
		if (!(_wfopen_s(&fiop, L"config.crn", L"r"))) {
			int tmpLength; // Temporary length variable

			/* Assigns configuration file data to application variables */

			// Username
			fread_s(&tmpLength, 4, 4, 1, fiop); // Gets length of file
			accountName = malloc(tmpLength * 2); // Allocates memory
			fread_s(accountName, tmpLength * 2, 2, tmpLength, fiop); // Reads length of file

			fclose(fiop); // Closes file
		}
		else {
			// If unable to open file, show error message box
			MessageBox(NULL, L"Unable to open configuration file", szTitle, MB_ICONERROR);
		}

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
#define START_STOP_MINER 1
#define ABOUT_DISPLAY 2
#define LICENSE_DISPLAY 3
#define LOAD_WALLETS 4
#define BROWSE_FOLDER 5
#define SAVE_CONFIG 6

// Child window handlers
HWND minerToggle, username, walletLb, timeDisplay, earningsDisplay, balanceDisplay, minerNameIn,
	saveLocationDisplay, storageAllocIn;

// Window procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE: // Creates child windows
		GetSystemInfo(&sysinfo); // Gets system info

		// Static text
		timeDisplay = CreateStaticText(L"00:00:00", 193, 182, 175, 18, hWnd); // Elapsed time display
		// Session earnings display
		earningsDisplay = CreateStaticText(L"18446744073.709551615 CryptX", WW_CENTER / 2 + 2, 220, 270, 18, hWnd);
		// Wallet balance display
		balanceDisplay = CreateStaticText(L"18446744073.709551615 CryptX", WW_CENTER / 2 + 2, 260, 270, 18, hWnd);
		// Save location display
		saveLocationDisplay = CreateStaticText(saveLocation, WW_CENTER + WW_CENTER / 2, 122, 270, 18, hWnd);

		// Text boxes
		username = CreateTextBox(accountName, 183, 62, 200, 18, hWnd); // Username text box
		minerNameIn = CreateTextBox(minerName, WW_CENTER + 183, 62, 195, 18, hWnd); // Miner name text box
		
		// Buttons
		// Start/Stop miner button
		minerToggle = CreateButton(L"Start miner", WW_CENTER - 78, W_HEIGHT - 70, 150, 30, hWnd, (HMENU)START_STOP_MINER);
		CreateButton(L"About CrunchNet", WW_CENTER - 78, W_HEIGHT - 20, 150, 30, hWnd, (HMENU)ABOUT_DISPLAY); // About CrunchNet button
		CreateButton(L"View license", WW_CENTER + 78, W_HEIGHT - 20, 150, 30, hWnd, (HMENU)LICENSE_DISPLAY); // View license button
		CreateButton(L"Load", 243, 132, 75, 30, hWnd, (HMENU)LOAD_WALLETS); // Load wallets button
		CreateButton(L"Browse", WW_CENTER + 180, 102, 150, 20, hWnd, (HMENU)BROWSE_FOLDER); // Browse folder button
		CreateButton(L"Save configuration", WW_CENTER + 78, W_HEIGHT - 70, 150, 30, hWnd, (HMENU)SAVE_CONFIG); // Save configuration button

		// List boxes
		walletLb = CreateListBox(107, 123, 190, 80, hWnd); // Wallet list box
		SendMessage(walletLb, LB_ADDSTRING, NULL, (LPARAM)&L"Select wallet..."); // Adds unselected option to list box

#define THREAD_COUNT sysinfo.dwNumberOfProcessors // CPU logical processer count macro

		// Trackbars
		break;
	case WM_COMMAND: // Handles messages from child windows
		switch (wParam) {
		case START_STOP_MINER: // Executes when Start/Stop miner button is pressed
			// Checks if miner is running
			if (!isMinerRunning) {
				StartMiner(hWnd, minerToggle); // If not running, start miner
			}
			else {
				StopMiner(hWnd, minerToggle); // If running, stop miner
			}
			break;
		case ABOUT_DISPLAY: // Executes when About CrunchNet button is pressed
			MessageBox(hWnd,
				L"CrunchNet Miner (Windows)\nVersion 1.0\nCopyright (c) 2018 Kade Burnside\n\nThis program is licensed under the GNU GPLv3.\nSource code is available at https://github.com/cadellapowell61/crunchnet \nTo get in touch with me, email me at cadellapowell61@gmail.com",
				L"About CrunchNet", MB_APPLMODAL);
			break;
		case LICENSE_DISPLAY: // Executes when View license button is pressed
			ShellExecute(NULL, NULL, L"https://www.gnu.org/licenses/gpl-3.0-standalone.html", NULL, NULL, NULL);
			break;
		case LOAD_WALLETS: // Executes when Load button is pressed
			break;
		case BROWSE_FOLDER: // Executes when Browse button is pressed
			// Checks if miner is active, and if active, show error message
			M_ACTIVE_ERR(hWnd) {
				BROWSEINFO bi = { 0 }; // Browse info structure
				LPITEMIDLIST pIdl; // Item identifier structure

				// Shows dialog
				bi.ulFlags = BIF_USENEWUI;
				OleInitialize(NULL);
				pIdl = SHBrowseForFolder(&bi);

				// Gets path from dialog
				if (pIdl != NULL) {
					ZeroMemory(&saveLocation, sizeof(WCHAR[_MAX_PATH]));
					if (SHGetPathFromIDList(pIdl, saveLocation) == 0) {
						MessageBox(hWnd, L"Invalid location!", szTitle, MB_ICONERROR | MB_APPLMODAL);
					}
					CoTaskMemFree(pIdl);
				}
				OleUninitialize();
				SetWindowText(saveLocationDisplay, saveLocation);
			}
			break;
		case SAVE_CONFIG: // Exeuctes when Save configuration button is pressed
			// // Checks if miner is active, and if active, show error message
			M_ACTIVE_ERR(hWnd) {
				// Opens configuration file
				if (!(_wfopen_s(&fiop, L"config.crn", L"w"))) {
					fclose(fiop); // Closes file
				}
				else {
					// If unable to open file, show error message box
					MessageBox(NULL, L"Unable to open configuration file", szTitle, MB_ICONERROR);
				}
			}
			break;
		}
		break;
	case WM_PAINT: // Draws GDI elements on window
		// Creates brushes
		HotPink = CreateSolidBrush(RGB(255, 105, 180));
		DeepPink = CreateSolidBrush(RGB(255, 20, 147));

		// Loads header font
		HFONT hFont = CreateFont(0, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, FF_SWISS, NULL);

		hdc = BeginPaint(hWnd, &ps);

		/* Shapes */

		// Deep pink
		SelectObject(hdc, DeepPink);
		Rectangle(hdc, 0, 0, W_WIDTH + 4, 30);
		Rectangle(hdc, 0, W_HEIGHT - 40, W_WIDTH + 4, W_HEIGHT);

		// Hot pink
		SelectObject(hdc, HotPink);
		Rectangle(hdc, 10, 50, WW_CENTER - 5, 150);
		Rectangle(hdc, 10, 170, WW_CENTER - 5, W_HEIGHT - 100);
		Rectangle(hdc, WW_CENTER + 5, 50, W_WIDTH - 6, W_HEIGHT - 100);
		Rectangle(hdc, 10, W_HEIGHT - 90, W_WIDTH - 6, W_HEIGHT - 50);

		/* Text */

		SetBkMode(hdc, TRANSPARENT); // Makes text border transparent
		SetTextAlign(hdc, TA_CENTER); // Makes text center-aligned
		SetTextColor(hdc, 0xffffff); // Makes text white

		// Default font text
		DisplayText(hdc, WW_CENTER / 2, 32, L"Account", 7); // Account header
		DisplayText(hdc, WW_CENTER / 2, 152, L"Earnings", 8); // Earnings header
		DisplayText(hdc, WW_CENTER + WW_CENTER / 2, 32, L"Miner configuration", 19); // Miner configuration header

		DisplayText(hdc, 46, 52, L"Username:", 9); // Username label
		DisplayText(hdc, 40, 67, L"Wallet:", 7); // Wallet label
		DisplayText(hdc, 243, 80, L"Reload\nwallets", 14); // Reload wallets label

		DisplayText(hdc, 57, 172, L"Elapsed time:", 13); // Elapsed time label
		DisplayText(hdc, WW_CENTER / 2, 192, L"CryptX earned in mining session", 31); // Miner earnings label
		DisplayText(hdc, WW_CENTER / 2, 232, L"Total wallet balance", 20); // Wallet balance label

		DisplayText(hdc, WW_CENTER + 45, 52, L"Miner name:", 11); // Miner name label
		DisplayText(hdc, WW_CENTER + WW_CENTER / 2, 72, L"Useful if you mine on multiple computers", 40); // Miner name description
		DisplayText(hdc, WW_CENTER + 52, 92, L"Save location:", 14); // Save location label
		DisplayText(hdc, WW_CENTER + 65, 132, L"Allocated storage:", 18); // Allocated storage label

		// Custom font text
		SelectObject(hdc, hFont);
		DisplayText(hdc, W_WIDTH / 2, 3, L"CrunchNet Miner Dashboard", 25); // Window header

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
