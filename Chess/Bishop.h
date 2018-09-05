#pragma once
#include "ChessPiece.h"
class Bishop :
	public ChessPiece
{
public:
	virtual void InitChessPiece(COLORTYPE cType, int x, int y);
	Bishop(COLORTYPE cType, int x, int y);
	~Bishop();
};

