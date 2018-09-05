#pragma once
#include "BitMap.h"
#include "BasicH.h"
#include "ChessPiece.h"

enum GAMESTATE
{
	GS_NORMALSTATE,
	GS_SELECTSTATE
};

class GameManager
{
	BitMap m_ClickedNoticeBitMap;
	BitMap m_chessBoardBitMaps[CT_END];
	GAMESTATE m_GameState;
	PIECETYPE m_ClickedPieceType;
	static GameManager* S;

	bool m_isGameOver;
	bool m_turn;
	int m_turnCount;
	static ChessPiece* m_chessPieces[PT_END];
	PIECETYPE m_chessPiecePosMatrix[8][8];

	void InitchessPiecePosMatrix();
	void DrawChessBoard(HDC& hdc);
	void DrawChessPieces(HDC& hdc);
	void MoveChesePieceAtMatrix(int x, int y, PIECETYPE PT);
	vector<POINT> GetPieceCanMovePos(PIECETYPE PT);

	GameManager();
public:
	void InitGameManager();
	void OperatorInput(int x, int y);
	void DrawGame(HDC& hdc);
	
	static GameManager* GetInstance();
	static void Release();

	~GameManager();
};

