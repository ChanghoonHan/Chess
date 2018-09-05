#pragma once
#include <Windows.h>
#include "BasicH.h"

class BitMap
{
	HDC m_hdc;
	HBITMAP m_NBitMap;
	HBITMAP m_OBitMap;

public:
	void Draw(HDC& hdc, int x, int y);
	void Init(string bitMapName);
	void Release();

	BitMap();
	~BitMap();
};

