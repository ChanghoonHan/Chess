#pragma once
#define SAFE_DELETE(a) if((a) != NULL) delete (a); (a)= NULL;
#include <string>
#include <vector>
using namespace std;

enum COLORTYPE
{
	CT_BLACK,
	CT_WHITE,
	CT_END
};

enum PIECETYPE
{
	PT_W_ROOK1,
	PT_W_KNIGHT1,
	PT_W_BISHOP1,
	PT_W_KING,
	PT_W_QUEEN,
	PT_W_BISHOP2,
	PT_W_KNIGHT2,
	PT_W_ROOK2,
	PT_W_PAWN1,
	PT_W_PAWN2,
	PT_W_PAWN3,
	PT_W_PAWN4,
	PT_W_PAWN5,
	PT_W_PAWN6,
	PT_W_PAWN7,
	PT_W_PAWN8,

	PT_B_PAWN1,
	PT_B_PAWN2,
	PT_B_PAWN3,
	PT_B_PAWN4,
	PT_B_PAWN5,
	PT_B_PAWN6,
	PT_B_PAWN7,
	PT_B_PAWN8,
	PT_B_ROOK1,
	PT_B_KNIGHT1,
	PT_B_BISHOP1,
	PT_B_KING,
	PT_B_QUEEN,
	PT_B_BISHOP2,
	PT_B_KNIGHT2,
	PT_B_ROOK2,
	PT_END
};