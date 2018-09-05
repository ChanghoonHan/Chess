#pragma once
#include "ChessPiece.h"

class Queen :
	public ChessPiece
{
public:
	virtual void InitChessPiece(COLORTYPE cType, int x, int y);
	Queen(COLORTYPE cType, int x, int y);
	~Queen();
};

