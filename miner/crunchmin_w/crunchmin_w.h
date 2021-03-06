/*
crunchmin_w.h: CrunchNet Miner extended source
Copyright (c) 2018 Mooseland Corporation
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

#ifndef CRUNCHMIN_W_H
#define CRUNCHMIN_W_H

// Internal CrunchNet includes
#include "crnminer_w.h"
#include "resource.h"

HINSTANCE ghInst; // Global HINSTANCE

// Window width & height
#define W_WIDTH 580
#define W_HEIGHT 370

// Window horizontal & vertical center values
#define WW_CENTER W_WIDTH / 2
#define WH_CENTER W_HEIGHT / 2

HBRUSH HeaderBrush, SectionBrush; // Colour brushes
RECT rect; // Drawing rect
HFONT hFont; // Font handle

// Control styles
#define DEFCONTROL WS_VISIBLE | WS_CHILD // Default control style

// Creates control
HWND CreateControl(LPCWSTR className, LPCWSTR text, DWORD style,
	int x, int y, int w, int h, HWND parent, HMENU action) {
	return CreateWindow(className, text, style, x - w / 2, y - h / 2, w, h, parent, action, ghInst, NULL);
}

// Creates static text
HWND CreateStaticText(LPCWSTR text, int x, int y, int w, int h, HWND parent) {
	return CreateControl(WC_STATIC, text, DEFCONTROL, x, y, w, h, parent, NULL);
}

// Creates text box
HWND CreateTextBox(LPCWSTR text, int x, int y, int w, int h, HWND parent) {
	return CreateControl(WC_EDIT, text, DEFCONTROL | ES_AUTOHSCROLL, x, y, w, h, parent, NULL);
}

// Creates button
HWND CreateButton(LPCWSTR text, int x, int y, int w, int h, HWND parent, HMENU action) {
	return CreateControl(WC_BUTTON, text, DEFCONTROL, x, y, w, h, parent, action);
}

// Creates list box
HWND CreateListBox(int x, int y, int w, int h, HWND parent) {
	return CreateControl(WC_LISTBOX, NULL, DEFCONTROL | LBS_STANDARD | LBS_HASSTRINGS, x, y, w, h, parent, NULL);
}

// Creates drop-down list
HWND CreateDropDownList(int x, int y, int w, int h, HWND parent) {
	return CreateControl(WC_COMBOBOX, NULL, DEFCONTROL | CBS_DROPDOWNLIST | CBS_HASSTRINGS, x, y, w, h, parent, NULL);
}

// Creates trackbar
HWND CreateTrackbar(int x, int y, int w, int h, HWND parent, int min, int max) {
	HWND track = CreateControl(TRACKBAR_CLASS, NULL, DEFCONTROL | TBS_AUTOTICKS, x, y, w, h, parent, NULL);
	SendMessage(track, TBM_SETRANGE, TRUE, (LPARAM)MAKELONG(min, max));
	SendMessage(track, TBM_SETPAGESIZE, 0, (LPARAM)4);
	SetFocus(track);
	return track;
}

// Displays text on screen
void DisplayText(HDC hdc, int x, int y, LPCWSTR text, int cch) {
	SetRect(&rect, x, y, x, y);
	DrawText(hdc, text, cch, &rect, DT_NOCLIP);
}

// Starts external application
void StartApplication(LPCWSTR file) {
	CreateProcess(file, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi);
}

#endif
