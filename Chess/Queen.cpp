#include "Queen.h"

void Queen::InitChessPiece(COLORTYPE cType, int x, int y)
{
	if (cType == CT_BLACK)
	{
		m_PieceBitMap.Init("block_b_04.bmp");
	}
	else
	{
		m_PieceBitMap.Init("block_w_04.bmp");
	}

	m_ClickedNoticeBitMap.Init("block02.bmp");
	m_isLive = true;
	m_isClicked = false;

	m_iX = x;
	m_iY = y;
}

Queen::Queen(COLORTYPE cType, int x, int y)
{
	InitChessPiece(cType, x, y);
}


Queen::~Queen()
{
}
