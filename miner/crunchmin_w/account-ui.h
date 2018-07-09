/*
account-ui.h: CrunchNet Miner account window UI source
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

#ifndef ACCOUNT_UI_H
#define ACCOUNT_UI_H

// Internal CrunchNet includes
#include "crunchmin_w.h"

// Window procedure declarations
LRESULT CALLBACK SignInProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SignUpProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SelectProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK PasswordProc(HWND, UINT, WPARAM, LPARAM);

// Initializes windows
void AccountWindowInit() {
	// Window structures
	WNDCLASS signInWc = { 0 };
	WNDCLASS signUpWc = { 0 };
	WNDCLASS selectWc = { 0 };
	WNDCLASS passwordWc = { 0 };

	// Window background brush
	HBRUSH winBackground;
	winBackground = CreateSolidBrush(RGB(64, 0, 64));

	/* Window structure attributes */

	// Sign in window
	signInWc.style = CS_HREDRAW | CS_VREDRAW;
	signInWc.lpfnWndProc = SignInProc;
	signInWc.hInstance = ghInst;
	signInWc.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(APPICON));
	signInWc.hCursor = LoadCursor(NULL, IDC_ARROW);
	signInWc.hbrBackground = winBackground;
	signInWc.lpszClassName = L"crunchmin_w_signin";

	// Sign up window
	signUpWc.style = CS_HREDRAW | CS_VREDRAW;
	signUpWc.lpfnWndProc = SignUpProc;
	signUpWc.hInstance = ghInst;
	signUpWc.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(APPICON));
	signUpWc.hCursor = LoadCursor(NULL, IDC_ARROW);
	signUpWc.hbrBackground = winBackground;
	signUpWc.lpszClassName = L"crunchmin_w_signup";

	// Account select window
	selectWc.style = CS_HREDRAW | CS_VREDRAW;
	selectWc.lpfnWndProc = SelectProc;
	selectWc.hInstance = ghInst;
	selectWc.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(APPICON));
	selectWc.hCursor = LoadCursor(NULL, IDC_ARROW);
	selectWc.hbrBackground = winBackground;
	selectWc.lpszClassName = L"crunchmin_w_select";

	// Password comfirmation window
	passwordWc.style = CS_HREDRAW | CS_VREDRAW;
	passwordWc.lpfnWndProc = PasswordProc;
	passwordWc.hInstance = ghInst;
	passwordWc.hIcon = LoadIcon(ghInst, MAKEINTRESOURCE(APPICON));
	passwordWc.hCursor = LoadCursor(NULL, IDC_ARROW);
	passwordWc.hbrBackground = winBackground;
	passwordWc.lpszClassName = L"crunchmin_w_password";

	/* Registers window structures */

	// Sign in window
	if (!RegisterClass(&signInWc)) {
		// If failed, show error message box
		MessageBox(NULL, L"Sign in window structure registration failed!", szTitle, MB_ICONERROR);
	}

	// Sign up window
	if (!RegisterClass(&signUpWc)) {
		// If failed, show error message box
		MessageBox(NULL, L"Sign up window structure registration failed!", szTitle, MB_ICONERROR);
	}

	// Account select window
	if (!RegisterClass(&selectWc)) {
		// If failed, show error message box
		MessageBox(NULL, L"Account select window structure registration failed!", szTitle, MB_ICONERROR);
	}

	// Password comfirmation window
	if (!RegisterClass(&passwordWc)) {
		// If failed, show error message box
		MessageBox(NULL, L"Password comfirmation window structure registration failed!", szTitle, MB_ICONERROR);
	}
}

/* Window creation functions */

// Sign in window
DWORD ShowSignIn() {
	// Creates window
	HWND hWnd = CreateWindow(L"crunchmin_w_signin", L"Sign in - CrunchNet Miner",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		300, 239, NULL, NULL, ghInst, NULL);

	// Checks if window was created successfully
	if (!hWnd) {
		// If not, show error message box
		MessageBox(NULL, L"Window creation failed!", szTitle, MB_ICONERROR);
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW); // Displays window
	UpdateWindow(hWnd); // Updates window

	MSG msg; // Message handle
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// Sign up window
DWORD ShowSignUp() {
	// Creates window
	HWND hWnd = CreateWindow(L"crunchmin_w_signup", L"Sign up - CrunchNet Miner",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		300, 339, NULL, NULL, ghInst, NULL);

	// Checks if window was created successfully
	if (!hWnd) {
		// If not, show error message box
		MessageBox(NULL, L"Window creation failed!", szTitle, MB_ICONERROR);
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW); // Displays window
	UpdateWindow(hWnd); // Updates window

	MSG msg; // Message handle
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// Account select window
DWORD ShowAccountSelect() {
	// Creates window
	HWND hWnd = CreateWindow(L"crunchmin_w_select", L"Select account - CrunchNet Miner",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		300, 379, NULL, NULL, ghInst, NULL);

	// Checks if window was created successfully
	if (!hWnd) {
		// If not, show error message box
		MessageBox(NULL, L"Window creation failed!", szTitle, MB_ICONERROR);
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW); // Displays window
	UpdateWindow(hWnd); // Updates window

	MSG msg; // Message handle
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// Password comfirmation window
DWORD ShowPasswordConfirm() {
	// Creates window
	HWND hWnd = CreateWindow(L"crunchmin_w_password", L"Re-enter password - CrunchNet Miner",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		300, 209, NULL, NULL, ghInst, NULL);

	// Checks if window was created successfully
	if (!hWnd) {
		// If not, show error message box
		MessageBox(NULL, L"Window creation failed!", szTitle, MB_ICONERROR);
		return 0;
	}

	ShowWindow(hWnd, SW_SHOW); // Displays window
	UpdateWindow(hWnd); // Updates window

	MSG msg; // Message handle
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

/* Window procedures */

// Sign in window
HWND signInUsername, signInPassword; // Window handles
LRESULT CALLBACK SignInProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE:
		signInUsername = CreateTextBox(NULL, 142, 85, 270, 20, hWnd); // Username text box
		SendMessage(signInUsername, EM_SETCUEBANNER, NULL, (LPARAM)&L"Username"); // Sets placeholder text
		signInPassword = CreateControl(WC_EDIT, NULL, DEFCONTROL | ES_AUTOHSCROLL | ES_PASSWORD, 142, 112, 270, 20, hWnd, NULL); // Username text box
		SendMessage(signInPassword, EM_SETCUEBANNER, NULL, (LPARAM)&L"Password"); // Sets placeholder text
		SendMessage(signInPassword, WM_SETFONT, NULL, (LPARAM)TRUE); // Makes password text correct size
		CreateButton(L"Sign in", 142, 150, 150, 30, hWnd, (HMENU)1); // Sign in button
		CreateButton(L"Create one!", 222, 180, 90, 20, hWnd, (HMENU)2); // Create account button
		break;
	case WM_COMMAND:
		switch (wParam) {
		case 1:
			DestroyWindow(hWnd);
			break;
		case 2:
			DestroyWindow(hWnd);
			ShowSignUp();
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SetBkMode(hdc, TRANSPARENT); // Makes text background transparent

		// Shapes
		SelectObject(hdc, CreateSolidBrush(0xffffff));
		Rectangle(hdc, 0, 65, 304, 70); // Horizontal line
		Rectangle(hdc, 0, 125, 304, 130); // Horizontal line

		// Text
		SetTextAlign(hdc, TA_CENTER); // Sets text align to center
		SetTextColor(hdc, 0xffffff); // Sets text colour to white
		DisplayText(hdc, 92, 172, L"Don't have an account?", 22); // Don't have an account?
		SelectObject(hdc, hFont);
		DisplayText(hdc, 142, 7, L"Sign in to your\nCrunchNet account", 33); // Sign in to your CrunchNet account

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

// Sign up window
HWND signUpFn, signUpMn, signUpSn, signUpUsername, signUpEmail,
signUpBd, signUpBm, signUpBy, signUpGender, signUpPronoun, signUpPassword; // Window handles
LRESULT CALLBACK SignUpProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE:
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Drawing code goes here
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

// Account select window
HWND selectView; // Window handles
LRESULT CALLBACK SelectProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE:
		break;
	case WM_COMMAND:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Drawing code goes here
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

// Password comfirmation window
HWND passwordPassword; // Window handles
LRESULT CALLBACK PasswordProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	switch (message) {
	case WM_CREATE:
		passwordPassword = CreateTextBox(NULL, 142, 117, 270, 20, hWnd); // Password text box
		SendMessage(passwordPassword, EM_SETCUEBANNER, NULL, (LPARAM)&L"Password"); // Adds placeholder text
		CreateButton(L"Submit password", 142, 147, 150, 30, hWnd, (HMENU)1); // Submit password button
		break;
	case WM_COMMAND:
		switch (wParam) {
		case 1:
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		SetBkMode(hdc, TRANSPARENT); // Makes text background transparent

		// Shapes
		SelectObject(hdc, CreateSolidBrush(0xffffff));
		Rectangle(hdc, 0, 35, 304, 40); // Horizontal line

		// Text
		SetTextAlign(hdc, TA_CENTER); // Sets text align to center
		SetTextColor(hdc, 0xffffff); // Sets text colour to white
		DisplayText(hdc, 148, 52, L"For security purposes, you are\nrequired to re-enter your password\nevery 30 days.", 80); // Notice
		SelectObject(hdc, hFont);
		DisplayText(hdc, 148, 7, L"Re-enter password", 17); // Re-enter password

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

#endif
