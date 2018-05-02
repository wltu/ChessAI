#include "ChessPiece.h"
#include <iostream>
using namespace std;

//Provide method to give legal moves
ChessPiece::ChessPiece(char type, int side, int posX, int posY) {
	this->type = type;
	this->side = side;

	this->onBoard = true;
	this->Selected = false;
	this->hasMoved = false;

	this->posX = posX;
	this->posY = posY;

	this->dangerWhite = false;
	this->dangerBlack = false;

	this->kingSpecial = false;
}

ChessPiece::ChessPiece() {
	this->type = 'n';
	this->side =  -1;
	this->onBoard = false;
	this->Selected = false;

	this->dangerWhite = false;
	this->dangerBlack = false;
	this->kingSpecial = false;
}

char ChessPiece::getType() {
	return this->type;
}
int ChessPiece::getSide() {
	return this->side;
}

void ChessPiece::setCastling(bool kingSpecial) {
	this->kingSpecial = kingSpecial;
}

void ChessPiece::setType(char type) {
	this->type = type;
}

void ChessPiece::move(ChessPiece board[8][8], int posX, int posY, float boardX[8], float boardY[8], int &turn) {
	if (posX != this->posX || posY != this->posY) {
		int ix, iy, fx, fy;
		ix = this->posX;
		iy = this->posY;

		ChessPiece temp = board[posX][posY];
		ChessPiece temp2 = *this;

		//Pawn Promotion.
		if (this->getType() == 'p') {
			if (this->getSide() == 0 && posY == 0) {

				this->setType('Q');
			}else if(this->getSide() == 1 && posY == 7){
				this->setType('Q');
			}

		}	


		this->hasMoved = true;
		board[posX][posY] = *this;
		board[posX][posY].posX = posX;
		board[posX][posY].posY = posY;

		
		board[ix][iy] = ChessPiece();

		fx = posX;
		fy = posY;


		//Castling
		if (this->kingSpecial) {

			board[posX][posY].kingSpecial = false;

			if (board[posX + 1][posY].getType() == 'r') {
				temp = board[posX + 1][posY]; //rook
				temp2 = board[posX - 1][posY];

				temp.hasMoved = true;
				board[posX - 1][posY] = temp;
				board[posX - 1][posY].posX = posX - 1;

				board[posX + 1][posY] = ChessPiece();

			}else if (board[posX - 2][posY].getType() == 'r') {
				temp = board[posX - 2][posY]; //rook
				temp2 = board[posX + 1][posY];

				temp.hasMoved = true;
				board[posX + 1][posY] = temp;
				board[posX + 1][posY].posX = posX + 1;
				board[posX -2][posY] = ChessPiece();
			}
			
		}

		board[ix][iy].Selected = true;
		board[fx][fy].Selected = true;
		
		//turn = !turn;
		
	}	
}
