/*
		
	Bazılari dünyannn sonu ateşle gelecek der, bazıları ise buzla.
	Anladığım kadar, benim aklım ateşten olanlara kanar.
	Ama iki kez yok olacaksa eğer, çoktandır nefreti de tattığımdan.
	Buzdan batmak da görkemli olacak.
	Nefretler ancak böyle sönecek...


		Ömer Furkan | 10.12.2017 | 07:45 PM
*/



#include "MineBot.h"


HWND hWnd;
LRESULT CALLBACK DlgProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
DWORD GetModuleBase(LPSTR lpModuleName, DWORD dwProcessId);
DWORD WINAPI PlaySound();
DWORD WINAPI StopSound();
DWORD WINAPI Mining();
DWORD WINAPI Move();
void EnableDebugPriv();
void GetLastErrorString();

HMODULE hKernel;
HMODULE hUser32;
//HMODULE hAutoIt;

HANDLE hHandle;
HWND hHeroHandle;
HWND hDlgProc;

//  0x0050B2F0 + 0x14
DWORD dwModuleBase;
DWORD dwTmp;

int WINAPI WinMain(
	_In_		HINSTANCE hInstance,
	_In_opt_	HINSTANCE hPrevInstance,
	_In_		LPWSTR    lpCmdLine,
	_In_		int       nCmdShow) {


	hKernel = LoadLibrary("kernel32.dll");
	hUser32 = LoadLibrary("user32.dll");
//	hAutoIt = LoadLibrary("AutoItX3.dll");


	EnableDebugPriv();
	HANDLE hThread = CreateThread(NULL, 0, PlaySound, NULL, 0, NULL);
	DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_MAIN1), hWnd, (DLGPROC)DlgProc);
}

LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	hDlgProc = hWndDlg;

	DWORD dwPid;
	DWORD dwMineValue = 0;
	DWORD dwSelectedItem = 0x0;
	DWORD dwTotalItem = 0x0;

	HICON hIcon;
	HICON hIconSm;

	HANDLE hMiningThread;
	HANDLE hMoveThread;

	FindWindowHook _FindWindowHook = (FindWindowHook)GetProcAddress(hUser32, "FindWindowA");
	GetWindowThreadProcessIdHook _GetWindowThreadProcessIdHook = (GetWindowThreadProcessIdHook)GetProcAddress(hUser32, "GetWindowThreadProcessId");
	OpenProcessHook _OpenProcessHook = (OpenProcessHook)GetProcAddress(hKernel, "OpenProcess");
	ReadProcessMemoryHook _ReadProcessMemoryHook = (ReadProcessMemoryHook)GetProcAddress(hKernel, "ReadProcessMemory");

	switch (Msg)
	{
	case WM_INITDIALOG:
		hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0x20, 0x20, 0x0);
		SendMessage(hWndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0x10, 0x10, 0x0);
		SendMessage(hWndDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIconSm);
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDC_ADDMINE:
			
			hHeroHandle = _FindWindowHook(NULL, "Hero Plus");
			if (!hHeroHandle) {
				MessageBoxA(hWndDlg, "Hero Plus Bulunamad�!", "Hata", MB_ICONERROR);
				break;
			}

			_GetWindowThreadProcessIdHook(hHeroHandle, &dwPid);
			hHandle = _OpenProcessHook(PROCESS_ALL_ACCESS, FALSE, dwPid);
			if (!hHandle) {
				MessageBoxA(hWndDlg, "OpenProcess Hata!", "Hata", MB_ICONERROR);
				return FALSE;
			}
			else {
				dwModuleBase = GetModuleBase("Hero.exe", dwPid);
				_ReadProcessMemoryHook(hHandle, (void *)(dwModuleBase + 0x0050B2F0), &dwTmp, sizeof(dwTmp), 0x0);
				_ReadProcessMemoryHook(hHandle, (void *)(dwTmp + 0x14), &dwMineValue, sizeof(dwMineValue), 0x0);
				char szBuffer[1024];
				wsprintfA(szBuffer, "%d", dwMineValue);
				SendDlgItemMessage(hWndDlg, IDC_MINELIST, LB_ADDSTRING, 0x0, (LPARAM)szBuffer);
			}
			break;
		case IDC_REMOVEMINE:
			dwSelectedItem = SendDlgItemMessage(hWndDlg, IDC_MINELIST, LB_GETANCHORINDEX, 0x0, 0x0);
			SendDlgItemMessage(hWndDlg, IDC_MINELIST, LB_DELETESTRING, (WPARAM)dwSelectedItem, 0x0);
			break;
		case IDC_START:
			StopSound();
			SetForegroundWindow(hHeroHandle);
			hMiningThread = CreateThread(NULL, 0, Mining, NULL, 0, NULL);
		//	hMoveThread = CreateThread(NULL, 0, Move, NULL, 0, NULL);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0x0);
		return TRUE;
	}

	return FALSE;
}


DWORD WINAPI PlaySound()
{
	uFMOD_PlaySong(MAKEINTRESOURCE(IDR_XM1), 0x0, XM_RESOURCE);
}

DWORD WINAPI StopSound() {
	uFMOD_StopSong();
}

void EnableDebugPriv()
{
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);

	CloseHandle(hToken);
}


void GetLastErrorString() {
	DWORD dwErr = GetLastError();
	if (dwErr == 0x0) {
		return;
	}

	LPSTR messageBuffer;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	MessageBoxA(NULL, (LPCSTR)messageBuffer, "Hata", MB_OK);
	LocalFree(messageBuffer);
}

DWORD GetModuleBase(LPSTR lpModuleName, DWORD dwProcessId)
{

	CreateToolhelp32SnapshotHook _CreateCreateToolhelp32SnapshotHook = (CreateToolhelp32SnapshotHook)GetProcAddress(hKernel, "CreateToolhelp32Snapshot");
	Module32FirstHook _Module32FirstHook = (Module32FirstHook)GetProcAddress(hKernel, "Module32First");
	Module32NextHook _Module32NextHook = (Module32NextHook)GetProcAddress(hKernel, "Module32Next");

	MODULEENTRY32 lpModuleEntry = { 0 };
	HANDLE hSnapShot = _CreateCreateToolhelp32SnapshotHook(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
//	GetLastErrorString();
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		GetLastErrorString();
		return NULL;
	}
	else {
		lpModuleEntry.dwSize = sizeof(lpModuleEntry);
		BOOL bModule = _Module32FirstHook(hSnapShot, &lpModuleEntry);
		while (bModule)
		{
			if (!strcmp(lpModuleEntry.szModule, lpModuleName))
			{
				CloseHandle(hSnapShot);
				return (DWORD)lpModuleEntry.modBaseAddr;
			}
			bModule = _Module32NextHook(hSnapShot, &lpModuleEntry);
		}
		CloseHandle(hSnapShot);
	}

	return NULL;
}



DWORD WINAPI Mining() {
	DWORD dwNumber;
	char cNewValue[MAX_PATH];
	char cCurrentValue[MAX_PATH];
	DWORD dwCurrentValue = 0;
	DWORD dwMineId;
	DWORD dwNewValue;
	DWORD dwTotalMine = SendDlgItemMessage(hDlgProc, IDC_MINELIST, LB_GETCOUNT, 0x0, 0x0);
	INPUT iKeyboard;

	WriteProcessMemoryHook _WriteProcessMemoryHook = (WriteProcessMemoryHook)GetProcAddress(hKernel, "WriteProcessMemory");
	ReadProcessMemoryHook _ReadProcessMemoryHook = (ReadProcessMemoryHook)GetProcAddress(hKernel, "ReadProcessMemory");
//	AU3_SendHook _AU3_SendHook = (AU3_SendHook)GetProcAddress(hAutoIt, "AU3_Send");

	iKeyboard.type = INPUT_KEYBOARD;
	iKeyboard.ki.wScan = 0x0;
	iKeyboard.ki.time = 0x0;
	iKeyboard.ki.dwExtraInfo = 0x0;

	iKeyboard.ki.wVk = 0x31;
	iKeyboard.ki.dwFlags = 0x0;
	

	DWORD *dwMineIDs = malloc(dwTotalMine);

	for (DWORD i = 0x0; i != dwTotalMine; i++) {
		SendDlgItemMessage(hDlgProc, IDC_MINELIST, LB_GETTEXT, i, (LPARAM)(LPSTR)cCurrentValue);
		dwMineIDs[i] = atoi(cCurrentValue);
		memset(cCurrentValue, 0x0, sizeof(cCurrentValue));
	}
	
	DWORD dwMin = dwMineIDs[0x0];
	DWORD dwMax = dwMineIDs[0x0];

	for (DWORD i = 0x0; i != dwTotalMine; i++) {
		if (dwMineIDs[i] > dwMax) {
			dwMax = dwMineIDs[i];
		}
		if (dwMineIDs[i] < dwMin) {
			dwMin = dwMineIDs[i];
		}
	}

	while (0x1) {
		_ReadProcessMemoryHook(hHandle, (void *)(dwTmp + 0x14), &dwMineId, sizeof(dwMineId), 0x0);
		if (dwMineId < dwMin || dwMineId > dwMax) {
			srand(time(NULL));
			dwNumber = rand() % dwTotalMine + 1;
			SendDlgItemMessage(hDlgProc, IDC_MINELIST, LB_GETTEXT, dwNumber, (LPARAM)(LPSTR)cNewValue);
			dwNewValue = atoi(cNewValue);
			_WriteProcessMemoryHook(hHandle, (void *)(dwTmp + 0x14), &dwNewValue, sizeof(dwNewValue), 0x0);
		}
		else {
			AU3_Send("1", 0x0);
			Sleep(2000);
		}
	}

}


DWORD WINAPI Move() {
	while (0x1) {
		Sleep(7000);
		AU3_Send("{UP 5}", 0x0);
	}
}