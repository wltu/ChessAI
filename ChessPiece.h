#ifndef CHESSPIECE_H
#define CHESSPIECE_H

using namespace std;

class ChessPiece
{
public:
	ChessPiece(char type, int side, float x, float y, int posX, int posY);
	ChessPiece();
	char getType();
	int getSide();
	void setType(char type);
	float getX();
	float getY();
	void setX(float x);
	void setY(float x);
	void setCastling(bool kingSpecial);

	bool onBoard;
	bool Selected;
	bool hasMoved;
	int posX;
	int posY;

	bool dangerWhite;
	bool dangerBlack;

	bool kingSpecial;

	void move(ChessPiece board[8][8], int posX, int posY, float boardX[8], float boardY[8], int &turn);

private:
	char type;
	int side; //0 is black 1 is white
	float x;
	float y;

};

#endif // CHESSPIECE_H
