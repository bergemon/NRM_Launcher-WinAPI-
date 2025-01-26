#include "bitmap.h"
#include "get_file_path.h"

//====================================================================
CUSTOM_BITMAP::CUSTOM_BITMAP()
{
	m_fileHeadSize = sizeof(BITMAPFILEHEADER);
	m_fileHead.bfType = 0x4d42;
	m_pInfoHead = nullptr;
	m_aBitmapBits = nullptr;
}
//====================================================================
CUSTOM_BITMAP::~CUSTOM_BITMAP()
{
	if (m_pInfoHead)
	{
		delete[] m_pInfoHead;
	}
	if (m_aBitmapBits)
	{
		delete[] m_aBitmapBits;
	}
	if (m_inpFile.is_open())
	{
		m_outFile.close();
	}
	if (m_outFile.is_open())
	{
		m_outFile.close();
	}
}
//====================================================================
BYTE* CUSTOM_BITMAP::GetBitmapBits()
{
	return m_aBitmapBits;
}
//====================================================================
BITMAPINFOHEADER* CUSTOM_BITMAP::GetInfoHeader()
{
	return m_pInfoHead;
}
//====================================================================
int CUSTOM_BITMAP::GetWidth()
{ 
	return m_width;
}
//====================================================================
int CUSTOM_BITMAP::GetHeight()
{
	return m_height;
}
//====================================================================
const wchar_t* CUSTOM_BITMAP::GetError()
{
	return m_error.c_str();
}
//====================================================================
BOOL CUSTOM_BITMAP::CreateDib24(int w, int h, const char* fileName)
{
	m_width = w;
	m_height = h;
	m_bytesPerLine = ((m_width * 24 + 63) / 64) * 8;
	m_imageSize = m_bytesPerLine * m_height;
	m_infoHeadSize = sizeof(BITMAPINFOHEADER);
	m_fileHead.bfSize = m_fileHeadSize + m_infoHeadSize + m_bytesPerLine * m_height;
	m_fileHead.bfOffBits = m_fileHeadSize + m_infoHeadSize;
	m_infoHead.biSize = m_infoHeadSize;
	m_infoHead.biWidth = m_width;
	m_infoHead.biHeight = m_height;
	m_infoHead.biPlanes = 1;
	m_infoHead.biBitCount = 24;
	m_infoHead.biCompression = BI_RGB;
	m_infoHead.biSizeImage = m_imageSize;
	m_aBitmapBits = new BYTE[m_imageSize];
	memset(m_aBitmapBits, 0, m_imageSize);
	m_outFile.open(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!m_outFile) return FALSE;
	else return TRUE;
}
//====================================================================
bool CUSTOM_BITMAP::CreateBkgMask(void(*mask_path)(std::string&))
{
	struct RGB
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	// Cut file name from path
	std::string file_name = m_fileName.c_str();
	cut_filename_from_btn_path(file_name);

	// Get full mask path of the background
	std::string mask_name;
	mask_path(mask_name);
	mask_name.append(file_name);

	std::filesystem::path mask_file = mask_name.c_str();
	
	if (!std::filesystem::exists(mask_file))
	{

		m_outFile.open(mask_name, std::ios::out | std::ios::binary);
		if (!m_outFile.is_open())
		{
			return false;
		}
		BYTE* mask_aBitmapBits = new BYTE[m_imageSize];

		int iter = 0;
		int color_count = 0;
		RGB pixel;
		uint32_t modImageSize = m_imageSize % 3;

		while (iter < m_imageSize - modImageSize)
		{
			// Bytes alignment
			uint32_t modBytesPerLine = m_bytesPerLine % 3;
			if (iter == (m_bytesPerLine - modBytesPerLine))
			{
				iter += modBytesPerLine;
			}

			pixel.red = m_aBitmapBits[iter++];
			pixel.green = m_aBitmapBits[iter++];
			pixel.blue = m_aBitmapBits[iter++];

			if (pixel.red == 0 && pixel.green == 0 && pixel.blue == 0)
			{
				mask_aBitmapBits[iter - 3] = 255;
				mask_aBitmapBits[iter - 2] = 255;
				mask_aBitmapBits[iter - 1] = 255;
			}
			else
			{
				mask_aBitmapBits[iter - 3] = 0;
				mask_aBitmapBits[iter - 2] = 0;
				mask_aBitmapBits[iter - 1] = 0;
			}
		}

		// BITMAPFILEHEADER
		m_outFile.write((const char*)&m_fileHead, m_fileHeadSize);

		// BITMAPINFOHEADER
		m_outFile.write((const char*)m_pInfoHead, m_infoHeadSize);

		// Image
		m_outFile.write((const char*)mask_aBitmapBits, m_imageSize);

		if (m_outFile.is_open())
		{
			m_outFile.close();
		}
	}

	return true;
}
//====================================================================
BOOL CUSTOM_BITMAP::LoadFromFile(const char* fileName)
{
	m_fileName = fileName;
	m_inpFile.open(fileName, std::ios::in | std::ios::binary);
	if (!m_inpFile)
	{
		m_error = L"Неверное имя файла или каталога.";
		return FALSE;
	}

	// BITMAPFILEHEADER
	m_inpFile.read((char*)&m_fileHead, m_fileHeadSize);
	if (m_fileHead.bfType != 0x4d42)
	{
		m_error = L"Это не BMP - файл";
		return FALSE;
	}

	// BITMAPINFOHEADER
	m_infoHeadSize = m_fileHead.bfOffBits - m_fileHeadSize;
	int fileSize = m_fileHead.bfSize;
	m_imageSize = fileSize - (m_fileHeadSize + m_infoHeadSize);
	m_pInfoHead = (BITMAPINFOHEADER*)(new BYTE[m_infoHeadSize]);
	m_inpFile.read((char*)m_pInfoHead, m_infoHeadSize);
	m_width = m_pInfoHead->biWidth;
	m_height = m_pInfoHead->biHeight;
	m_aBitmapBits = new BYTE[m_imageSize];

	m_inpFile.read((char*)m_aBitmapBits, m_imageSize);
	return true;
}
//====================================================================
int CUSTOM_BITMAP::Draw(HDC hdc, int xDst, int yDst, int wDst, int hDst, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop)
{
	return StretchDIBits(hdc, xDst, yDst, wDst, hDst, xSrc, ySrc, wSrc, hSrc,
		m_aBitmapBits, (CONST BITMAPINFO*)m_pInfoHead, DIB_RGB_COLORS, rop);
}
//====================================================================
void CUSTOM_BITMAP::SetPixel(int x, int y, COLORREF color)
{
	int row = y;
	int col = 3 * x;
	m_aBitmapBits[row * m_bytesPerLine + col] = GetBValue(color);
	m_aBitmapBits[row * m_bytesPerLine + col + 1] = GetGValue(color);
	m_aBitmapBits[row * m_bytesPerLine + col + 2] = GetRValue(color);
}
//====================================================================
void CUSTOM_BITMAP::StoreDib24()
{
	m_outFile.write((char*)&m_fileHead, m_fileHeadSize);
	m_outFile.write((char*)&m_infoHead, m_infoHeadSize);
	m_outFile.write((char*)m_aBitmapBits, m_imageSize);
}
//====================================================================