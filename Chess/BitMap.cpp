#include "BitMap.h"

void BitMap::Init(string bitMapName)
{
	if (m_NBitMap == NULL)
	{
		m_NBitMap = (HBITMAP)LoadImage(NULL, bitMapName.c_str(), IMAGE_BITMAP, 0, 0,
			LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	}
}

void BitMap::Draw(HDC& hdc, int x, int y)
{
	m_hdc = CreateCompatibleDC(hdc);
	m_OBitMap = (HBITMAP)SelectObject(m_hdc, m_NBitMap);
	TransparentBlt(hdc, x, y, 100, 100, m_hdc, 0, 0, 125, 125, RGB(255, 0, 255));
	(HBITMAP)SelectObject(m_hdc, m_OBitMap);
	DeleteDC(m_hdc);
}

void BitMap::Release()
{
	DeleteObject(m_NBitMap);
}

BitMap::BitMap()
{
	m_NBitMap = NULL;
}

BitMap::~BitMap()
{
	Release();
}
