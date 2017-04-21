#include "move.h"

Move::Move(int x, int y, int currentX, int currentY, char type) {
	this->x = x;
	this->y = y;
	this->cx = currentX;
	this->cy = currentY;
	this->type = type;
}

Move::Move(){
}

char Move::getType() {
	return this->type;
}

int Move::getX() {
	return this->x;
}

int Move::getY() {
	return this->y;
}

int Move::getCX() {
	return this->cx;
}

int Move::getCY() {
	return this->cy;
}