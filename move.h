#ifndef MOVE_H
#define MOVE_H

class Move {
	public:
		Move(int x, int y, int currentX, int currentY, char type);
		Move();
		int getX();
		int getY();
		int getCX();
		int getCY();

		char getType();

	private:
		int x;
		int y;
		int cx;
		int cy;
		char type;

};

#endif