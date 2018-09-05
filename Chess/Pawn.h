#pragma once
#include "ChessPiece.h"

class Pawn :
	public ChessPiece
{
public:
	virtual void InitChessPiece(COLORTYPE cType, int x, int y);
	Pawn(COLORTYPE cType, int x, int y);
	virtual ~Pawn();
};

