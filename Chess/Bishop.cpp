#include "Bishop.h"

void Bishop::InitChessPiece(COLORTYPE cType, int x, int y)
{
	if (cType == CT_BLACK)
	{
		m_PieceBitMap.Init("block_b_02.bmp");
	}
	else
	{
		m_PieceBitMap.Init("block_w_02.bmp");
	}

	m_ClickedNoticeBitMap.Init("block02.bmp");
	m_isLive = true;
	m_isClicked = false;

	m_iX = x;
	m_iY = y;
}

Bishop::Bishop(COLORTYPE cType, int x, int y)
{
	InitChessPiece(cType, x, y);
}


Bishop::~Bishop()
{
}
