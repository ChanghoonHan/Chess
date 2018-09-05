#pragma once
#include "ChessPiece.h"

class Rook :
	public ChessPiece
{
public:
	virtual void InitChessPiece(COLORTYPE cType, int x, int y);
	Rook(COLORTYPE cType, int x, int y);
	~Rook();
};

