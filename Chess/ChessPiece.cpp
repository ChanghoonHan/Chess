#include "ChessPiece.h"

void ChessPiece::SetCanMovePos(vector<POINT>CMP)
{
	m_canMovePos = CMP;
}

void ChessPiece::Draw(HDC hdc)
{
	if (!m_isLive)
	{
		return;
	}

	if (m_isClicked)
	{
		m_ClickedNoticeBitMap.Draw(hdc, m_iX, m_iY);

		m_PieceBitMap.Draw(hdc, 800, 50);
		for (auto iter = m_canMovePos.begin(); iter != m_canMovePos.end(); iter++)
		{
			m_ClickedNoticeBitMap.Draw(hdc, iter->x*100, iter->y*100);
		}
	}

	m_PieceBitMap.Draw(hdc, m_iX, m_iY);
}

bool ChessPiece::Move(int x, int y)
{
	if (m_iX == (x / 100) * 100 && m_iY == (y / 100) * 100)
	{
		return false;
	}

	for (auto iter = m_canMovePos.begin(); iter != m_canMovePos.end(); iter++)
	{
		if (x / 100 == iter->x && y / 100 == iter->y)
		{
			m_iX = (x / 100)*100;
			m_iY = (y / 100)*100;
			m_isClicked = false;
			return true;
			break;
		}
	}

	return false;
}

void ChessPiece::KillPiece()
{
	m_isLive = false;
}

void ChessPiece::UnClick()
{
	m_isClicked = false;
}

bool ChessPiece::CheckisClicked(int x, int y)
{
	if (!m_isLive)
	{
		return false;
	}

	if (x >= m_iX && x <= m_iX + 100 && y >= m_iY && y <= m_iY + 100)
	{
		m_isClicked = true;
		return true;
	}

	return false;
}

ChessPiece::ChessPiece()
{
}


ChessPiece::~ChessPiece()
{
}
