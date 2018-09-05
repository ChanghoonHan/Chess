#pragma once
#include "ChessPiece.h"
class Knight :
	public ChessPiece
{
public:
	virtual void InitChessPiece(COLORTYPE cType, int x, int y);
	Knight(COLORTYPE cType, int x, int y);
	~Knight();
};

