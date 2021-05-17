#include <Windows.h>
#include <iostream>

RECT rect;
LPDWORD lpThreadId;

static struct MyData {
	HANDLE bmp;
	HANDLE text;
	int number;
};

DWORD WINAPI ThreadFunctionClipboard(LPVOID);

int main() {
	HWND hWnd = GetForegroundWindow();
	if (!OpenClipboard(hWnd)) {
		std::cout << "ERROR: невозможно открыть буфер обмена" << std::endl;
		return 1;
	}
	if (!EmptyClipboard()) {
		std::cout << "ERROR: невозможно очистить буфер обмена" << std::endl;
		return 2;
	}
#pragma region Текстовый файл
	HANDLE hTextFile = CreateFileW(
		L"text.txt",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	HANDLE hTextMappingFile = CreateFileMapping(
		hTextFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		L"text"
	);

	HANDLE hTextFileStartAddr = (HANDLE)MapViewOfFile(
		hTextMappingFile,
		FILE_MAP_READ,
		0,
		0,
		0
	);
#pragma endregion

#pragma region BMP файл
	HANDLE hBmpFile = CreateFileA(
		"file.bmp",
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	GetClientRect(hWnd, &rect);
	HANDLE hBmpMappingFile = CreateFileMappingA(
		hBmpFile,
		NULL,
		PAGE_READONLY,
		0,
		0,
		"file"
	);

	HANDLE hBmpMapFileAddr = MapViewOfFile(
		hBmpMappingFile,
		FILE_MAP_READ,
		0,
		0,
		0
	);
#pragma endregion
	MyData data;
	data.text = hTextFileStartAddr;
	data.number = 7;
	data.bmp = hBmpMapFileAddr;
	UINT format = RegisterClipboardFormat(L"CUSTOM");

	//std::cout << "Text File-" << data.text << " " << (char*)data.text << std::endl;
	//std::cout << "Number-" << data.number << std::endl;
	//std::cout << "Bmp-" << data.bmp << std::endl;
	HGLOBAL hGl = NULL;
	hGl = GlobalAlloc(GMEM_DDESHARE, sizeof(MyData));
	MyData* buffer = (MyData*)GlobalLock(hGl);
	*buffer = data;
	GlobalUnlock(hGl);
	SetClipboardData(format, hGl);
	CloseClipboard();

	HANDLE hThread = CreateThread(NULL, 0, &ThreadFunctionClipboard, 0, NULL, lpThreadId);
	WaitForSingleObject(hThread, INFINITE);
	return 0;
}

DWORD WINAPI ThreadFunctionClipboard(LPVOID lParam) {
	std::cout << "Work first thread!" << std::endl;
	HWND hWnd = GetForegroundWindow();

	UINT format = RegisterClipboardFormat(L"CUSTOM");
	MyData data;
	if (OpenClipboard(hWnd))
	{
		//std::cout << "SSSS" << data.bmp << std::endl;
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
	}
	std::cin.get();
	ExitProcess(0);
}