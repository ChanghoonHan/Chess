#pragma once
#include "BitMap.h"

class ChessPiece
{
protected:
	BitMap m_PieceBitMap;
	BitMap m_ClickedNoticeBitMap;
	vector<POINT> m_canMovePos;
	bool m_isLive;
	bool m_isClicked;
	int m_iX;
	int m_iY;
	
public:
	void KillPiece();
	void SetCanMovePos(vector<POINT>CMP );
	void ChessPiece::UnClick();
	virtual void InitChessPiece(COLORTYPE cType, int x, int y) = 0;
	void Draw(HDC hdc);
	bool Move(int x, int y);
	bool CheckisClicked(int x, int y);

	ChessPiece();
	virtual ~ChessPiece();
};

