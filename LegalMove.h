#ifndef LEGAL_MOVE_H
#define LEGAL_MOVE_H

#include "ChessPiece.h"

class LegalMove
{
public:
	LegalMove();
	void Hightlight(ChessPiece board[8][8], int x, int y, bool danger);
	void action(ChessPiece board[8][8], int ix, int iy, int x, int y, bool danger, int side);
	void setDangers(ChessPiece board[8][8]);

	void RookMove(ChessPiece board[8][8], int x, int y, int num, int side, bool danger);
	void BishopMove(ChessPiece board[8][8], int x, int y, int num, int side, bool danger);
	void Castling(ChessPiece board[8][8], int x, int y);
	void resetHighlight(ChessPiece board[8][8]);

	ChessPiece board2[8][8];

	int whiteKingX = 4;
	int whiteKingY = 7;
	int blackKingX = 4;
	int blackKingY = 0;

	void moveWhiteKing(int x, int y);
	void moveBlackKing(int x, int y);

	bool GameOver = false;
private:
	int i;
};

#endif
