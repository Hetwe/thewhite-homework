#include <Windows.h>
#include <iostream>

static RECT rect;
static HWND hWnd;

static struct MyData {
	HANDLE bmp;
	HANDLE text;
	int number;
};

DWORD WINAPI ThreadFunctionClipboard(LPVOID);
HANDLE OpenMyFile(LPCWSTR, LPCWSTR);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	hWnd = GetForegroundWindow();

	if (!OpenClipboard(hWnd)) {
		std::cout << "ERROR: Невозможно открыть буфер обмена!";
		return 1;
	}
    EmptyClipboard(); 
	
	MyData data;
	data.text = OpenMyFile(L"text.txt", L"text");
	data.number = 7;
	data.bmp = OpenMyFile(L"file.bmp", L"bmp");
	UINT format = RegisterClipboardFormat(L"CUSTOM");

	HGLOBAL hGl = NULL;
	hGl = GlobalAlloc(GMEM_DDESHARE, sizeof(MyData));
	MyData* buffer = (MyData*)GlobalLock(hGl);
	*buffer = data;
	GlobalUnlock(hGl);

	SetClipboardData(format, hGl);
	CloseClipboard();

	HANDLE hThread = CreateThread(NULL, 0, &ThreadFunctionClipboard, 0, NULL, NULL);
	WaitForSingleObject(hThread, INFINITE);
	return 0;
}

HANDLE OpenMyFile(LPCWSTR pathToFile, LPCWSTR nameFile){
	HANDLE hFile = CreateFileW(
		pathToFile,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
    	std::cout << "ERROR: Невозможно создать файл!";
	}

	HANDLE hMapFile = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		nameFile
	);
	if (hMapFile == NULL || hMapFile == INVALID_HANDLE_VALUE) 
   	{ 
    	std::cout << "ERROR: Невозможно создать отражение в памяти!";
   	}
	HANDLE pBuf = MapViewOfFile(
		hMapFile,
		FILE_MAP_READ,
		0,
		0,
		0
	);
	if (pBuf == NULL) 
   	{ 
    	std::cout << "Представление проецированного файла невозможно!";
   	}else{
		return pBuf;   
	}
}

DWORD WINAPI ThreadFunctionClipboard(LPVOID lParam) {
	GetClientRect(hWnd, &rect);
	UINT format = RegisterClipboardFormat(L"CUSTOM");
	MyData data;
	OpenClipboard(hWnd);

	HANDLE hData = GetClipboardData(format);
	MyData* buffer = (MyData*)GlobalLock(hData);
	data = *buffer;

	BITMAPFILEHEADER* bFileHeader = (BITMAPFILEHEADER*)data.bmp;
	BITMAPINFO* bInfo = (BITMAPINFO*)((char*)data.bmp + 14);
	HDC hdc = GetDC(hWnd);
	HANDLE hBmpFile = CreateDIBitmap(
		hdc,
		&(bInfo->bmiHeader),
		CBM_INIT,
		(char*)data.bmp + bFileHeader->bfOffBits,
		bInfo,
		DIB_PAL_COLORS
	);
	HDC hMemDC = CreateCompatibleDC(hdc);
	SelectObject(hMemDC, hBmpFile);
	StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hMemDC,
		0, 0, bInfo->bmiHeader.biWidth, bInfo->bmiHeader.biHeight, SRCCOPY);
	ReleaseDC(hWnd, hdc);
	DeleteDC(hMemDC);
	DeleteObject(hBmpFile);
	GlobalUnlock(hData);
	CloseClipboard();
	std::cout << "Text File-" << data.text << " " << (char*)data.text << std::endl;
	std::cout << "Number-" << data.number << std::endl;
	std::cout << "Bmp-" << data.bmp << std::endl;
	std::cin.get();
	ExitThread(0);
}