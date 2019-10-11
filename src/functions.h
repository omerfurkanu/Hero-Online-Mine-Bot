#pragma once


typedef HANDLE(WINAPI *CreateToolhelp32SnapshotHook)(
	_In_ DWORD dwFlags,
	_In_ DWORD th32ProcessID
);

typedef BOOL(WINAPI *Module32FirstHook)(
	_In_    HANDLE          hSnapshot,
	_Inout_ LPMODULEENTRY32 lpme
);

typedef BOOL(WINAPI *Module32NextHook)(
	_In_  HANDLE          hSnapshot,
	_Out_ LPMODULEENTRY32 lpme
);

typedef HWND(WINAPI *FindWindowHook)(
	_In_opt_ LPCTSTR lpClassName,
	_In_opt_ LPCTSTR lpWindowName
);


typedef DWORD(WINAPI *GetWindowThreadProcessIdHook)(
	_In_      HWND    hWnd,
	_Out_opt_ LPDWORD lpdwProcessId
);

typedef HANDLE(WINAPI *OpenProcessHook)(
	_In_ DWORD dwDesiredAccess,
	_In_ BOOL  bInheritHandle,
	_In_ DWORD dwProcessId
);

typedef BOOL(WINAPI *ReadProcessMemoryHook) (
	_In_  HANDLE  hProcess,
	_In_  LPCVOID lpBaseAddress,
	_Out_ LPVOID  lpBuffer,
	_In_  SIZE_T  nSize,
	_Out_ SIZE_T  *lpNumberOfBytesRead
);

typedef BOOL(WINAPI *WriteProcessMemoryHook)(
	_In_  HANDLE  hProcess,
	_In_  LPVOID  lpBaseAddress,
	_In_  LPCVOID lpBuffer,
	_In_  SIZE_T  nSize,
	_Out_ SIZE_T  *lpNumberOfBytesWritten
);

typedef void(WINAPI *AU3_SendHook)(
	LPCWSTR szSendText,
	int nMode
);