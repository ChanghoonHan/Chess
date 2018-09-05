#pragma once
#include "ChessPiece.h"
class King :
	public ChessPiece
{
public:
	virtual void InitChessPiece(COLORTYPE cType, int x, int y);
	King(COLORTYPE cType, int x, int y);
	~King();
};

