#include "GameManager.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Pawn.h"

GameManager* GameManager::S = NULL;
ChessPiece* GameManager::m_chessPieces[PT_END] = { NULL, };

void GameManager::InitchessPiecePosMatrix()
{
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			m_chessPiecePosMatrix[i][j] = PT_END;

		}
	}

	for (size_t i = 0; i < 16; i++)
	{
		m_chessPiecePosMatrix[i / 8][i % 8] = (PIECETYPE)i;
	}

	for (size_t i = 0; i < 16; i++)
	{
		m_chessPiecePosMatrix[6+(i / 8)][i % 8] = (PIECETYPE)(i+16);
	}
}

void GameManager::InitGameManager()
{
	m_GameState = GS_NORMALSTATE;
	m_turnCount = 0;
	m_isGameOver = false;
	m_turn = CT_BLACK;
	InitchessPiecePosMatrix();
	m_chessBoardBitMaps[CT_WHITE].Init("block00.bmp");
	m_chessBoardBitMaps[CT_BLACK].Init("block01.bmp");
	m_ClickedNoticeBitMap.Init("block02.bmp");

	for (size_t i = 0; i < 8; i++)
	{
		m_chessPieces[PT_W_PAWN1 + i] = new Pawn(CT_WHITE, (i % 8) * 100, 100);
	}

	m_chessPieces[PT_W_ROOK1] = new Rook(CT_WHITE, 0, 0);
	m_chessPieces[PT_W_KNIGHT1] = new Knight(CT_WHITE, 100, 0);
	m_chessPieces[PT_W_BISHOP1] = new Bishop(CT_WHITE, 200, 0);
	m_chessPieces[PT_W_KING] = new King(CT_WHITE, 300, 0);
	m_chessPieces[PT_W_QUEEN] = new Queen(CT_WHITE, 400, 0);
	m_chessPieces[PT_W_BISHOP2] = new Bishop(CT_WHITE, 500, 0);
	m_chessPieces[PT_W_KNIGHT2] = new Knight(CT_WHITE, 600, 0);
	m_chessPieces[PT_W_ROOK2] = new Rook(CT_WHITE, 700, 0);

	for (size_t i = 0; i < 8; i++)
	{
		m_chessPieces[PT_B_PAWN1+i] = new Pawn(CT_BLACK, (i%8)*100, 600);
	}
	m_chessPieces[PT_B_ROOK1] = new Rook(CT_BLACK, 0, 700);
	m_chessPieces[PT_B_KNIGHT1] = new Knight(CT_BLACK, 100, 700);
	m_chessPieces[PT_B_BISHOP1] = new Bishop(CT_BLACK, 200, 700);
	m_chessPieces[PT_B_KING] = new King(CT_BLACK, 300, 700);
	m_chessPieces[PT_B_QUEEN] = new Queen(CT_BLACK, 400, 700);
	m_chessPieces[PT_B_BISHOP2] = new Bishop(CT_BLACK, 500, 700);
	m_chessPieces[PT_B_KNIGHT2] = new Knight(CT_BLACK, 600, 700);
	m_chessPieces[PT_B_ROOK2] = new Rook(CT_BLACK, 700, 700);
}

vector<POINT> GameManager::GetPieceCanMovePos(PIECETYPE PT)
{
	vector<POINT> canMovePosV;
	POINT piecePos;
	POINT canMovePos;

	for (size_t y = 0; y < 8; y++)
	{
		for (size_t x = 0; x < 8; x++)
		{
			if (m_chessPiecePosMatrix[y][x] == PT)
			{
				piecePos.x = x;
				piecePos.y = y;
			}
		}
	}

	switch (PT)
	{
	case PT_W_PAWN1:
	case PT_W_PAWN2:
	case PT_W_PAWN3:
	case PT_W_PAWN4:
	case PT_W_PAWN5:
	case PT_W_PAWN6:
	case PT_W_PAWN7:
	case PT_W_PAWN8:
		if (m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x] == PT_END)//바로 앞 이동 체크
		{
			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y + 1;
			canMovePosV.push_back(canMovePos);

			if (piecePos.y == 1 &&
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x] == PT_END)//두칸 앞 이동 체크
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y + 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.x - 1 > -1 && piecePos.y + 1 < 8)//왼쪽 아래 먹을 수 있는가
		{
			if (m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 1] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 1] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - 1;
				canMovePos.y = piecePos.y + 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.x + 1 < 8 && piecePos.y + 1 < 8)//오른쪽 아래 먹을 수 있는가
		{
			if (m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 1] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 1] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + 1;
				canMovePos.y = piecePos.y + 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		return canMovePosV;
		break;

	case PT_W_ROOK1:
	case PT_W_ROOK2:
		for (size_t i = 0; i < 7; i++)//6시
		{
			if (piecePos.y + (i + 1) > 7 || 
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_W_ROOK1 && 
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//12시
		{
			if (piecePos.y - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//3시
		{
			if (piecePos.x + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//9시
		{
			if (piecePos.x - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		return canMovePosV;
		break;
		
	case PT_W_KNIGHT1:
	case PT_W_KNIGHT2:
		if (piecePos.y - 2 > -1 && piecePos.x + 1 < 8) //1시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x + 1] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x + 1] <= PT_B_ROOK2)||
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x + 1] == PT_END)
			{
				canMovePos.x = piecePos.x + 1;
				canMovePos.y = piecePos.y - 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y - 2 > -1 && piecePos.x -1 > -1) //11시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x - 1] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x - 1] <= PT_B_ROOK2)||
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x - 1] == PT_END)
			{
				canMovePos.x = piecePos.x - 1;
				canMovePos.y = piecePos.y - 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y -1 > -1  && piecePos.x + 2 < 8) //2시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 2 ] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 2] <= PT_B_ROOK2)||
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 2] == PT_END)
			{
				canMovePos.x = piecePos.x + 2;
				canMovePos.y = piecePos.y - 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 1 < 8 && piecePos.x + 2 < 8) //4시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 2] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 2] <= PT_B_ROOK2) ||
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 2] == PT_END)
			{
				canMovePos.x = piecePos.x + 2;
				canMovePos.y = piecePos.y + 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 2 < 8 && piecePos.x + 1 < 8) //5시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x + 1] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x + 1] <= PT_B_ROOK2) ||
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x + 1] == PT_END)
			{
				canMovePos.x = piecePos.x + 1;
				canMovePos.y = piecePos.y + 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 2 < 8 && piecePos.x -1 > -1) //7시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x - 1] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x - 1] <= PT_B_ROOK2) ||
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x - 1] == PT_END)
			{
				canMovePos.x = piecePos.x - 1;
				canMovePos.y = piecePos.y + 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 1 < 8 && piecePos.x - 2 > -1) //8시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 2] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 2] <= PT_B_ROOK2) ||
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 2] == PT_END)
			{
				canMovePos.x = piecePos.x - 2;
				canMovePos.y = piecePos.y + 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y - 1 > -1 && piecePos.x - 2 > -1) //10시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 2] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 2] <= PT_B_ROOK2) ||
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 2] == PT_END)
			{
				canMovePos.x = piecePos.x - 2;
				canMovePos.y = piecePos.y - 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		return canMovePosV;
		break;

	case PT_W_BISHOP1:
	case PT_W_BISHOP2:
		for (size_t i = 0; i < 7; i++)//↘체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//↗체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↖체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↙체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		return canMovePosV;
		break;

	case PT_W_KING:
	{
		int direct[8][2] = { { -1, 0 },{ -1, 1 },{ 0, 1 },{ 1, 1 },{ 1, 0 },{ 1, -1 },{ 0, -1 },{ -1, -1 } };
		for (size_t i = 0; i < 8; i++)
		{
			if (piecePos.y + direct[i][0] > -1 && piecePos.y + direct[i][0] < 8 && piecePos.x + direct[i][1] > -1 && piecePos.x + direct[i][1] < 8 &&
				((m_chessPiecePosMatrix[piecePos.y + direct[i][0]][piecePos.x + direct[i][1]] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + direct[i][0]][piecePos.x + direct[i][1]] <= PT_B_ROOK2) ||
					m_chessPiecePosMatrix[piecePos.y + direct[i][0]][piecePos.x + direct[i][1]] == PT_END))
			{
				canMovePos.x = piecePos.x + direct[i][1];
				canMovePos.y = piecePos.y + direct[i][0];
				canMovePosV.push_back(canMovePos);
			}
		}
		return canMovePosV;
		break;
	}

	case PT_W_QUEEN:
		for (size_t i = 0; i < 7; i++)//↘체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//↗체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↖체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↙체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//6시
		{
			if (piecePos.y + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//12시
		{
			if (piecePos.y - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//3시
		{
			if (piecePos.x + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//9시
		{
			if (piecePos.x - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_W_PAWN8))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_B_ROOK2)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		return canMovePosV;
		break;

	case PT_B_PAWN1:
	case PT_B_PAWN2:
	case PT_B_PAWN3:
	case PT_B_PAWN4:
	case PT_B_PAWN5:
	case PT_B_PAWN6:
	case PT_B_PAWN7:
	case PT_B_PAWN8:
		if (m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x] == PT_END)//바로 앞 이동 체크
		{
			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y - 1;
			canMovePosV.push_back(canMovePos);

			if (piecePos.y == 6 &&
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x] == PT_END)//두칸 앞 이동 체크
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y - 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.x - 1 > -1 && piecePos.y - 1 > -1)//왼쪽 위 먹을 수 있는가
		{
			if (m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 1] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 1] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - 1;
				canMovePos.y = piecePos.y - 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.x + 1 < 8 && piecePos.y - 1 > -1)//오른쪽 위 먹을 수 있는가
		{
			if (m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 1] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 1] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + 1;
				canMovePos.y = piecePos.y - 1;
				canMovePosV.push_back(canMovePos);
			}
		}
		
		return canMovePosV;
		break;

	case PT_B_ROOK1:
	case PT_B_ROOK2:
		for (size_t i = 0; i < 7; i++)//6시
		{
			if (piecePos.y + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//12시
		{
			if (piecePos.y - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//3시
		{
			if (piecePos.x + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//9시
		{
			if (piecePos.x - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		return canMovePosV;
		break;

	case PT_B_KNIGHT1:
	case PT_B_KNIGHT2:
		if (piecePos.y - 2 > -1 && piecePos.x + 1 < 8) //1시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x + 1] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x + 1] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x + 1] == PT_END)
			{
				canMovePos.x = piecePos.x + 1;
				canMovePos.y = piecePos.y - 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y - 2 > -1 && piecePos.x - 1 > -1) //11시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x - 1] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x - 1] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y - 2][piecePos.x - 1] == PT_END)
			{
				canMovePos.x = piecePos.x - 1;
				canMovePos.y = piecePos.y - 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y - 1 > -1 && piecePos.x + 2 < 8) //2시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 2] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 2] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x + 2] == PT_END)
			{
				canMovePos.x = piecePos.x + 2;
				canMovePos.y = piecePos.y - 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 1 < 8 && piecePos.x + 2 < 8) //4시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 2] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 2] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x + 2] == PT_END)
			{
				canMovePos.x = piecePos.x + 2;
				canMovePos.y = piecePos.y + 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 2 < 8 && piecePos.x + 1 < 8) //5시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x + 1] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x + 1] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x + 1] == PT_END)
			{
				canMovePos.x = piecePos.x + 1;
				canMovePos.y = piecePos.y + 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 2 < 8 && piecePos.x - 1 > -1) //7시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x - 1] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x - 1] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y + 2][piecePos.x - 1] == PT_END)
			{
				canMovePos.x = piecePos.x - 1;
				canMovePos.y = piecePos.y + 2;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y + 1 < 8 && piecePos.x - 2 > -1) //8시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 2] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 2] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y + 1][piecePos.x - 2] == PT_END)
			{
				canMovePos.x = piecePos.x - 2;
				canMovePos.y = piecePos.y + 1;
				canMovePosV.push_back(canMovePos);
			}
		}

		if (piecePos.y - 1 > -1 && piecePos.x - 2 > -1) //10시방향체크
		{
			if ((m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 2] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 2] <= PT_W_PAWN8) ||
				m_chessPiecePosMatrix[piecePos.y - 1][piecePos.x - 2] == PT_END)
			{
				canMovePos.x = piecePos.x - 2;
				canMovePos.y = piecePos.y - 1;
				canMovePosV.push_back(canMovePos);
			}
		}
		return canMovePosV;
		break;

	case PT_B_BISHOP1:
	case PT_B_BISHOP2:
		for (size_t i = 0; i < 7; i++)//↘체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//↗체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↖체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x - (i + 1) < 0)||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↖체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		return canMovePosV;
		break;
	
	case PT_B_KING:
	{
		int direct[8][2] = { { -1, 0 }, { -1, 1 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 }, { 0, -1 }, { -1, -1 } };
		for (size_t i = 0; i < 8; i++)
		{
			if (piecePos.y + direct[i][0] > -1 && piecePos.y + direct[i][0] < 8 && piecePos.x + direct[i][1] > -1 && piecePos.x + direct[i][1] < 8 &&
				((m_chessPiecePosMatrix[piecePos.y + direct[i][0]][piecePos.x + direct[i][1]] >= PT_W_ROOK1 &&
					m_chessPiecePosMatrix[piecePos.y + direct[i][0]][piecePos.x + direct[i][1]] <= PT_W_PAWN8) ||
					m_chessPiecePosMatrix[piecePos.y + direct[i][0]][piecePos.x + direct[i][1]] == PT_END))
			{
				canMovePos.x = piecePos.x + direct[i][1];
				canMovePos.y = piecePos.y + direct[i][0];
				canMovePosV.push_back(canMovePos);
			}
		}
		return canMovePosV;
		break;
	}

	case PT_B_QUEEN:
		for (size_t i = 0; i < 7; i++)//↘체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x + (i + 1) > 7) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//↗체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x + (i + 1) >= 7) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x + (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↖체크
		{
			if ((piecePos.y - (i + 1) < 0 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}

		for (size_t i = 0; i < 7; i++)//↖체크
		{
			if ((piecePos.y + (i + 1) > 7 || piecePos.x - (i + 1) < 0) ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x - (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//6시
		{
			if (piecePos.y + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y + (i + 1)][piecePos.x] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y + (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y + (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//12시
		{
			if (piecePos.y - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y - (i + 1)][piecePos.x] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x;
				canMovePos.y = piecePos.y - (i + 1);
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x;
			canMovePos.y = piecePos.y - (i + 1);
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//3시
		{
			if (piecePos.x + (i + 1) > 7 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x + (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x + (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x + (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		for (size_t i = 0; i < 7; i++)//9시
		{
			if (piecePos.x - (i + 1) < 0 ||
				(m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_B_PAWN1 &&
					m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_B_ROOK2))
			{
				break;
			}

			if (m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] >= PT_W_ROOK1 &&
				m_chessPiecePosMatrix[piecePos.y][piecePos.x - (i + 1)] <= PT_W_PAWN8)
			{
				canMovePos.x = piecePos.x - (i + 1);
				canMovePos.y = piecePos.y;
				canMovePosV.push_back(canMovePos);
				break;
			}

			canMovePos.x = piecePos.x - (i + 1);
			canMovePos.y = piecePos.y;
			canMovePosV.push_back(canMovePos);
		}
		return canMovePosV;
		break;

	case PT_END:
		break;

	default:
		break;
	}
	return canMovePosV;
}

void GameManager::DrawChessBoard(HDC& hdc)
{
	for (size_t i = 0; i < 64; i++)
	{
		if ((i / 8) % 2 == 0)
		{
			if (i % 2 == 0)
			{
				m_chessBoardBitMaps[CT_WHITE].Draw(hdc, (i % 8) * 100, (i / 8) * 100);
			}
			else
			{
				m_chessBoardBitMaps[CT_BLACK].Draw(hdc, (i % 8) * 100, (i / 8) * 100);
			}
		}
		else
		{
			if (i % 2 == 0)
			{
				m_chessBoardBitMaps[CT_BLACK].Draw(hdc, (i % 8) * 100, (i / 8) * 100);
			}
			else
			{
				m_chessBoardBitMaps[CT_WHITE].Draw(hdc, (i % 8) * 100, (i / 8) * 100);
			}
		}
	}
}

void GameManager::DrawChessPieces(HDC& hdc)
{
	for (size_t j = 0; j < PT_END; j++)
	{
		m_chessPieces[j]->Draw(hdc);
	}
}

void GameManager::MoveChesePieceAtMatrix(int x, int y, PIECETYPE PT)
{
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			if (m_chessPiecePosMatrix[i][j] == PT)
			{
				m_chessPiecePosMatrix[i][j] = PT_END;
			}
		}
	}

	if (m_chessPiecePosMatrix[y / 100][x / 100] != PT_END)
	{
		if (m_chessPiecePosMatrix[y / 100][x / 100] == PT_W_KING ||
				m_chessPiecePosMatrix[y / 100][x / 100] == PT_B_KING)
		{
			m_isGameOver = true;
		}

		m_chessPieces[m_chessPiecePosMatrix[y / 100][x / 100]]->KillPiece();
	}
	m_chessPiecePosMatrix[y / 100][x / 100] = PT;
}

void GameManager::OperatorInput(int x, int y)
{
	if (m_isGameOver)
	{
		return;
	}

	bool isClickedPiece = false;

	if (m_GameState == GS_SELECTSTATE)
	{
		if (m_chessPieces[m_ClickedPieceType]->Move(x, y))
		{
			MoveChesePieceAtMatrix(x, y, m_ClickedPieceType);
			m_GameState = GS_NORMALSTATE;
			m_turn = !m_turn;
			m_turnCount++;
		}
	}

	if (m_turn == CT_WHITE)
	{
		for (size_t i = 0; i < PT_B_PAWN1; i++)
		{
			if (m_chessPieces[i]->CheckisClicked(x, y))
			{
				isClickedPiece = true;
				m_ClickedPieceType = (PIECETYPE)i;
				m_GameState = GS_SELECTSTATE;
				m_chessPieces[i]->SetCanMovePos(GetPieceCanMovePos(m_ClickedPieceType));
				break;
			}
		}
	}
	else
	{
		for (size_t i = PT_B_PAWN1; i < PT_END; i++)
		{
			if (m_chessPieces[i]->CheckisClicked(x, y))
			{
				isClickedPiece = true;
				m_ClickedPieceType = (PIECETYPE)i;
				m_GameState = GS_SELECTSTATE;
				m_chessPieces[i]->SetCanMovePos(GetPieceCanMovePos(m_ClickedPieceType));
				break;
			}
		}
	}

	if (isClickedPiece)
	{
		for (size_t i = 0; i < PT_END; i++)
		{
			if (i == m_ClickedPieceType)
			{
				continue;
			}
			m_chessPieces[i]->UnClick();
		}
	}
}

void GameManager::DrawGame(HDC& hdc)
{
	char buf[256];
	if (m_isGameOver)
	{
		if (m_turn == CT_WHITE)
		{
			sprintf(buf, "Turn : BlackWin!!\nTurnCount : %d", m_turnCount);
		}
		else
		{
			sprintf(buf, "Turn : WhiteWin!!\nTurnCount : %d", m_turnCount);
		}
	}
	else
	{
		if (m_turn == CT_WHITE)
		{
			sprintf(buf, "Turn : White\nTurnCount : %d", m_turnCount);
		}
		else
		{
			sprintf(buf, "Turn : Black\nTurnCount : %d", m_turnCount);
		}
	}
	

	m_ClickedNoticeBitMap.Draw(hdc, 800, 50);
	
	RECT rect = { 800, 0, 930, 50 };
	DrawText(hdc, buf, strlen(buf), &rect, DT_WORDBREAK | DT_LEFT);
	DrawChessBoard(hdc);
	DrawChessPieces(hdc);
}

GameManager* GameManager::GetInstance()
{
	if (S == NULL)
	{
		S = new GameManager;
	}

	return S;
}

void GameManager::Release()
{
	for (size_t j = 0; j < PT_END; j++)
	{
		SAFE_DELETE(m_chessPieces[j]);
	}

	SAFE_DELETE(S);
}

GameManager::GameManager()
{
}


GameManager::~GameManager()
{
}
