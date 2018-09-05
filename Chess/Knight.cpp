#include "Knight.h"

void Knight::InitChessPiece(COLORTYPE cType, int x, int y)
{
	if (cType == CT_BLACK)
	{
		m_PieceBitMap.Init("block_b_01.bmp");
	}
	else
	{
		m_PieceBitMap.Init("block_w_01.bmp");
	}

	m_ClickedNoticeBitMap.Init("block02.bmp");
	m_isClicked = false;

	m_iX = x;
	m_iY = y;
}

Knight::Knight(COLORTYPE cType, int x, int y)
{
	InitChessPiece(cType, x, y);
}


Knight::~Knight()
{
}
