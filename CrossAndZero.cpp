#include <windows.h>
#include <tchar.h>
#include "resource.h"
#include <fstream>


BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
void Step(HWND, UINT, HBITMAP, UINT&, TCHAR);
TCHAR Logic(TCHAR[3][3], TCHAR);
BOOL Win(TCHAR, HWND,TCHAR[3][3],UINT[3]);
BOOL ADraw(TCHAR[3][3], HWND,UINT[3]);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszCmdLine, INT nCmdShow) {

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}


BOOL CALLBACK DlgProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {

	static UINT uCounter = 0;
	static HBITMAP hBitmap = NULL;
	static HCURSOR hCursor = NULL;
	static HINSTANCE hInst = GetModuleHandle(NULL);
	static HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));

	static UINT uStat[3] = { 0,0,0 };

	static TCHAR pathStatFile[] = TEXT("D:\\stat.txt");
	static TCHAR symbol = TEXT('x');

	static TCHAR szMessage[30];
	static TCHAR szThePlayingField[3][3]{
	{TEXT(' '),TEXT(' '),TEXT(' ')},
	{TEXT(' '),TEXT(' '),TEXT(' ')},
	{TEXT(' '),TEXT(' '),TEXT(' ')}
	};


	static std::fstream file;


	if (uCounter % 2 == 0) {
		symbol = TEXT('x');
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		SetClassLong(hWnd, GCL_HCURSOR, LONG(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR10))));
	}
	else {
		symbol = TEXT('0');
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		SetClassLong(hWnd, GCL_HCURSOR, LONG(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR14))));
	}


	if (Logic(szThePlayingField, symbol) != TEXT(' ')) {
		if (Win(symbol, hWnd, szThePlayingField, uStat)) {

			if (symbol == TEXT('x')) {
				wsprintf(szMessage, TEXT("%d"), uStat[0]);
				SetDlgItemText(hWnd, IDC_STATIC1, szMessage);
				SetDlgItemText(hWnd, IDC_STATICRES, TEXT("Winner is 'x'"));
			}
			else {
				wsprintf(szMessage, TEXT("%d"), uStat[1]);
				SetDlgItemText(hWnd, IDC_STATIC2, szMessage);
				SetDlgItemText(hWnd, IDC_STATICRES, TEXT("Winner is '0'"));
			}

		}
	}
	else if (ADraw(szThePlayingField, hWnd, uStat)) {
		SetDlgItemText(hWnd, IDC_STATICRES, TEXT("A draw"));
		wsprintf(szMessage, TEXT("%d"), uStat[2]);
		SetDlgItemText(hWnd, IDC_STATIC3, szMessage);
	}


	switch (uMessage) {

	case WM_CLOSE:
		//Save stat of game in file
		file.open(pathStatFile);
		file << uStat[0] << '\n' << uStat[1] << '\n' << uStat[2];
		file.close();
		//Destr main icon
		DestroyIcon(hIcon);
		//End program
		EndDialog(hWnd, NULL);
		return true;

	case WM_INITDIALOG:
		file.open(pathStatFile);

		if (!file) {
			std::ofstream created(pathStatFile);
			created << 0 << '\n' << 0 << '\n' << 0;
			created.close();
		}

		file >> uStat[0] >> uStat[1] >> uStat[2];
		file.close();

		for (int i = 0; i < 3; ++i) {
			wsprintf(szMessage, TEXT("%d"), uStat[i]);
			SetDlgItemText(hWnd, IDC_STATIC1 + i, szMessage);
		}

		//Init main icon
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		//My buttonID 1009-1017 or IDC_BUTTON1-IDC_BUTTON9
		for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
			EnableWindow(GetDlgItem(hWnd, i), false);
		}
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDC_BTNSTART:
			SetWindowText(hWnd, TEXT("Cross&Zero"));

			//My buttonID 1009-1017 or IDC_BUTTON1-IDC_BUTTON9
			for (int i = IDC_BUTTON1; i <= IDC_BUTTON9; ++i) {
				EnableWindow(GetDlgItem(hWnd, i), true);
				SendMessage(GetDlgItem(hWnd, i), BM_SETIMAGE,
					WPARAM(IMAGE_BITMAP), NULL);
			}

			EnableWindow(GetDlgItem(hWnd, IDC_BTNSTART), false);
			EnableWindow(GetDlgItem(hWnd, IDC_RADIO1), false);
			EnableWindow(GetDlgItem(hWnd, IDC_RADIO2), false);
			return true;

		case IDC_RADIO1:
			uCounter = 0;
			return true;

		case IDC_RADIO2:
			uCounter = 1;
			return true;

		case IDC_BUTTON1:
			szThePlayingField[0][0] = symbol;
			Step(hWnd, IDC_BUTTON1, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON2:
			szThePlayingField[0][1] = symbol;
			Step(hWnd, IDC_BUTTON2, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON3:
			szThePlayingField[0][2] = symbol;
			Step(hWnd, IDC_BUTTON3, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON4:
			szThePlayingField[1][0] = symbol;
			Step(hWnd, IDC_BUTTON4, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON5:
			szThePlayingField[1][1] = symbol;
			Step(hWnd, IDC_BUTTON5, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON6:
			szThePlayingField[1][2] = symbol;
			Step(hWnd, IDC_BUTTON6, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON7:
			szThePlayingField[2][0] = symbol;
			Step(hWnd, IDC_BUTTON7, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON8:
			szThePlayingField[2][1] = symbol;
			Step(hWnd, IDC_BUTTON8, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON9:
			szThePlayingField[2][2] = symbol;
			Step(hWnd, IDC_BUTTON9, hBitmap, uCounter, symbol);
			return true;

		case IDC_BUTTON10:
			uStat[0] = uStat[1] = uStat[2] = 0;
			wsprintf(szMessage, TEXT("%d"), 0);
			for (int i = IDC_STATIC1; i <= IDC_STATIC3; ++i) {
				SetDlgItemText(hWnd, i, szMessage);
			}
			return true;
		}
		return true;
	}
	return false;
}


void Step(HWND hWnd, UINT buttonID, HBITMAP hBitmap, UINT& uCounter, TCHAR symbol) {

	static TCHAR szMessage[20];

	SendMessage(GetDlgItem(hWnd, buttonID),
		BM_SETIMAGE,
		WPARAM(IMAGE_BITMAP),
		LPARAM(hBitmap));

	if (symbol != TEXT('x')) {
		SetWindowText(hWnd, TEXT("Step of 'x'"));
	}
	else {
		SetWindowText(hWnd, TEXT("Step of '0'"));
	}

	EnableWindow(GetDlgItem(hWnd, buttonID), false);
	uCounter++;
}


BOOL ADraw(TCHAR szThePlayingField[3][3], HWND hWnd, UINT uStat[3]) {

	static UINT uCounter = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (szThePlayingField[i][j] == TEXT(' ')) {
				++uCounter;
			}
		}
	}

	if (uCounter == 0) {

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				szThePlayingField[i][j] = TEXT(' ');
			}
		}

		EnableWindow(GetDlgItem(hWnd, IDC_RADIO1), true);
		EnableWindow(GetDlgItem(hWnd, IDC_RADIO2), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BTNSTART), true);

		uStat[2]++;
		return true;
	}
	uCounter = 0;
	return false;
}


BOOL Win(TCHAR symbol, HWND hWnd, TCHAR szThePlayingField[3][3], UINT uStat[3]) {

	if (symbol != TEXT(' ')) {

		for (int i = 0; i <= IDC_BUTTON9; ++i) {
			EnableWindow(GetDlgItem(hWnd, i), false);
		}

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				szThePlayingField[i][j] = TEXT(' ');
			}
		}

		EnableWindow(GetDlgItem(hWnd, IDC_RADIO1), true);
		EnableWindow(GetDlgItem(hWnd, IDC_RADIO2), true);
		EnableWindow(GetDlgItem(hWnd, IDC_BTNSTART), true);

		if (symbol == TEXT('x')) {
			uStat[0]++;
		}
		else {
			uStat[1]++;
		}

		return true;
	}
	return false;
}


TCHAR Logic(TCHAR szThePlayingField[3][3], TCHAR symbol) {

	static UINT uCounter = 0;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (szThePlayingField[i][j] == symbol) {
				++uCounter;
				if (uCounter == 3) {
					return symbol;
				}
			}
		}
		uCounter = 0;
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (szThePlayingField[j][i] == symbol) {
				++uCounter;
				if (uCounter == 3) {
					return symbol;
				}
			}
		}
		uCounter = 0;
	}

	if (szThePlayingField[0][0] == symbol &&
		szThePlayingField[1][1] == symbol &&
		szThePlayingField[2][2] == symbol) {
		return symbol;
	}
	else if (szThePlayingField[0][2] == symbol &&
		szThePlayingField[1][1] == symbol &&
		szThePlayingField[2][0] == symbol) {
		return symbol;
	}
	return TEXT(' ');
}