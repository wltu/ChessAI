#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ChessPiece.h"
#include "LegalMove.h"
#include "move.h"
#include "MoveList.h"

//Random Numbers
#include <stdlib.h>  
#include <time.h>

#include <windows.h>
#include <iostream>


class Board {
public:
	Board();
	void draw();
	void fixedSize(int w, int h);
	void mouseInput(int button, int state, int x, int y);
	void setPiece(ChessPiece piece, float x, float y);
	void drawChessBase(float x, float y);
	void DrawCircle(float cx, float cy, float r, int num_segments, float zoom);
	void setUp(); //Set up the board with all chess pieces.
	void setBoard();
	int getSize();

	Node* SaveTheKing(ChessPiece piece); //remove any highlighted move that will put the king in danger.
	void ClearDanger();
	void CheckGameStatus();

	void AIMove();
	Move currentMove;
	Move nextMove;
	int moveCount;

	bool whiteWin = false;
	bool blackWin = false;

	//AI Methods
	bool AIGames = true; // change it later so it is optional.
	Move bestMove(int level, int side);
	Move Opening(Move start);
	
	int turn;
	bool PieceSeleced;
	
	ChessPiece current;

	void reset();

private:
	const int size = 640;
	bool start;
	float boardPositionX[8];
	float boardPositionY[8];
	ChessPiece pieces[8][8]; //Change to [8][8] actual board sizes
	LegalMove checker; // Check legal moves.

	int AILevel;
	int opening;
	Move firstMove;
};

#endif
