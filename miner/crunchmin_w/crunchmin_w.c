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

// Internal CrunchNet include
#include "crunchmin_w.h"

LPCWSTR szTitle = L"CrunchNet Miner"; // Default window title

// Forward declaration of WndProc function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point of application
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	si.cb = sizeof(STARTUPINFO); // Assigns startup info structure size to si.cb
	// Checks for update
	if (CheckForUpdate(&wsa)) {
		MessageBox(NULL, L"An update is required", L"CrunchNet Updater", NULL); // Shows message box
		// Starts CrunchNet Updater
		CreateProcess(L"crnupd_w.exe", NULL, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
		return 0; // Terminates application
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
#define PATH_DISPLAY 6
#define SAVE_CONFIG 7

// Child window handlers
HWND minerToggle;

// Window procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE: // Creates child windows
		GetSystemInfo(&sysinfo); // Gets system info

		// Static text

		// Text boxes

		// Buttons

		// List boxes

		// Trackbars
		break;
	case WM_COMMAND: // Handles messages from child windows
		switch (wParam) {
		case START_STOP_MINER: // Executes when Start/Stop miner button is pressed
			break;
		case ABOUT_DISPLAY: // Executes when About CrunchNet button is pressed
			MessageBox(hWnd,
				L"CrunchNet Miner (Windows)\nVersion 1.0\nCopyright (c) 2018 Kade Burnside\n\nThis program is licensed under the GNU GPLv3.\nSource code is available at github.com/cadellapowell61/crunchnet\nTo get in touch with me, email me at cadellapowell61@gmail.com",
				L"About CrunchNet", MB_APPLMODAL);
			break;
		case LICENSE_DISPLAY: // Executes when View license button is pressed
			ShellExecute(hWnd, L"https://www.gnu.org/licenses/gpl-3.0-standalone.html", NULL, NULL, NULL, NULL);
			break;
		case LOAD_WALLETS: // Executes when Load button is pressed
			break;
		case BROWSE_FOLDER: // Executes when Browse button is pressed
			break;
		case PATH_DISPLAY: // Executes when Current location button is pressed
			break;
		case SAVE_CONFIG: // Exeuctes when Save configuration button is pressed
			break;
		}
		break;
	case WM_PAINT: // Draws GDI elements on window
		// Creates brushes
		HotPink = CreateSolidBrush(RGB(255, 105, 180));
		DeepPink = CreateSolidBrush(RGB(255, 20, 147));

		hdc = BeginPaint(hWnd, &ps);

		// Shapes
		SelectObject(hdc, DeepPink);
		Rectangle(hdc, 0, 0, W_WIDTH + 4, 30);
		Rectangle(hdc, 0, W_HEIGHT - 40, W_WIDTH + 4, W_HEIGHT);

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
