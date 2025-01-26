#pragma once
#include <windows.h>
#include "stdlibs.h"
#include "const.h"

class CUSTOM_BITMAP
{
public:
	CUSTOM_BITMAP();
	~CUSTOM_BITMAP();
	BOOL CreateDib24(int w, int h, const char* fileName);
	bool CreateBkgMask(void(*mask_path)(std::string&));
	void StoreDib24();
	BOOL LoadFromFile(const char* fileName);
	void SetPixel(int x, int y, COLORREF color);
	int Draw(HDC hdc, int xDst, int yDst, int wDst, int hDst, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop);
	BYTE* GetBitmapBits();
	BITMAPINFOHEADER* GetInfoHeader();
	int GetWidth();
	int GetHeight();
	const wchar_t* GetError();

private:
	int m_width;
	int m_height;
	int m_bytesPerLine;
	BITMAPFILEHEADER m_fileHead;
	BITMAPINFOHEADER m_infoHead;
	BITMAPINFOHEADER* m_pInfoHead;
	BYTE* m_aBitmapBits;
	int m_fileHeadSize;
	int m_infoHeadSize;
	int m_imageSize;
	std::wstring m_error;
	std::ifstream m_inpFile;
	std::ofstream m_outFile;
	std::string m_fileName;
	bool m_isClear;
};