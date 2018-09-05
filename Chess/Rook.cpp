#include "Rook.h"

void Rook::InitChessPiece(COLORTYPE cType, int x, int y)
{
	if (cType == CT_BLACK)
	{
		m_PieceBitMap.Init("block_b_03.bmp");
	}
	else
	{
		m_PieceBitMap.Init("block_w_03.bmp");;
	}

	m_ClickedNoticeBitMap.Init("block02.bmp");
	m_isLive = true;
	m_isClicked = false;

	m_iX = x;
	m_iY = y;
}

Rook::Rook(COLORTYPE cType, int x, int y)
{
	InitChessPiece(cType, x, y);
}


Rook::~Rook()
{
}
