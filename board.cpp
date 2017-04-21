#include "board.h"


Board::Board() {
	this->start = true;
	this->PieceSeleced = false;
	checker = LegalMove();
	this->turn = 0;
	this->moveCount = 0;
}

void Board::draw() {
	if (start == true) {
		setUp();

		start = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float x1, x2, y1, y2;

	x1 = -1.0;
	x2 = 1.0;
	y1 = -1.0;
	y2 = 1.0;


	//Background
	glBegin(GL_QUADS);
	glColor3f(0.89f, 0.64f, 0.21f);
	glVertex3f(x1, y1, -1.0f);
	glVertex3f(x2, y1, -1.0f);
	glVertex3f(x2, y2, -1.0f);
	glVertex3f(x1, y2, -1.0f);
	glEnd();

	//Board
	x1 = -1;
	x2 = -1 + 0.25;
	y1 = 1 - 0.25;
	y2 = 1;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[j][i].Selected && pieces[j][i].onBoard) {
				glColor3f(0.63f, 0.82f, 0.89f);
				
			}else {
				if ((i - j) % 2 == 0) {
					glColor3f(0.85f, 0.65f, 0.40f);
				}
				else {
					glColor3f(0.43f, 0.25f, 0.0f);
				}
			}

			glBegin(GL_QUADS);
				glVertex3f(x1, y1, -2.9f);
				glVertex3f(x2, y1, -2.9f);
				glVertex3f(x2, y2, -2.9f);
				glVertex3f(x1, y2, -2.9f);;
			glEnd();

			if(pieces[j][i].Selected && !pieces[j][i].onBoard) {
				glColor3f(0.63f, 0.82f, 0.89f);

				DrawCircle(x1 + 0.125, y1 + 0.125, 0.05, 1000, -2.9);
			}


			x1 += 0.25;
			x2 += 0.25;

		}
		x1 = -1;
		x2 = -1 + 0.25;
		y1 -= 0.25;
		y2 -= 0.25;
	}

	setBoard();
	checker.setDangers(pieces);

	glutSwapBuffers();
}


void Board::fixedSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).

	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);


	// Force a fixed size window.
	glutReshapeWindow(640, 640);
}

void Board::mouseInput(int button, int state, int x, int y) {
	if (!checker.GameOver) {
		if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
			checker.resetHighlight(pieces);
			PieceSeleced = false;
			draw();
		}

		if (x >= 55 && x <= 585 && y >= 55 && y <= 585) {
			int cx = (x - 55) / 66.375;
			int cy = (y - 55) / 66.375;

			if (PieceSeleced) {
				if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {

					if (pieces[cx][cy].Selected) {
						if (current.getType() == 'K') {
							if (current.getSide() == 0) {
								checker.moveWhiteKing(cx, cy);
							}
							else {
								checker.moveBlackKing(cx, cy);
							}
						}

						if (current.posX != cx || current.posY != cy) {
							if (pieces[cx][cy].getType() == 'K') {
								if (pieces[cx][cy].getSide() == 0) {
									blackWin = true;
								}
								else {
									whiteWin = true;
								}
							}

							current.move(pieces, cx, cy, boardPositionX, boardPositionY, turn);

							CheckGameStatus();

							if (AIGames && !blackWin && ! whiteWin) {
								currentMove = Move(cx,cy, current.posX, current.posY, current.getType());

								AIMove();

								moveCount++;
							}

						}

					}
					else {
						checker.resetHighlight(pieces);
					}
					

					PieceSeleced = false;

					draw();

					CheckGameStatus();

				}
			}
			else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {

				if (pieces[cx][cy].onBoard && turn == pieces[cx][cy].getSide()) { //&& turn == pieces[cx][cy].getSide() add later. after testing
					checker.resetHighlight(pieces);
					checker.Hightlight(pieces, cx, cy, false);
					this->current = pieces[cx][cy];

					PieceSeleced = true;

					SaveTheKing(current);
				}

				draw();
			}
		}
		else {
			checker.resetHighlight(pieces);
			PieceSeleced = false;

			draw();
		}
	}
	
}

void Board::setBoard() {

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j].onBoard == true) {
				setPiece(pieces[i][j], pieces[i][j].getX(), pieces[i][j].getY());
			}

			pieces[i][j].dangerBlack = false;
			pieces[i][j].dangerWhite = false;
		}
	}
}

void Board::setPiece(ChessPiece piece, float x, float y) {
	
	if (piece.getSide() == 0) {
		glColor3f(1.0f, 1.0f, 1.0f);
	}
	else {
		glColor3f(0.0f, 0.0f, 0.0f);
	}

	switch (piece.getType()) {
		case 'p':
			//Pawn
			DrawCircle(0 + x, 0.49 + y, 0.193, 1000, -8.0);
			break;

		case 'r':
			//Rook
			glBegin(GL_QUADS);
				glVertex3f(0.12 + x, 0.30 + y, -8.0f);
				glVertex3f(-0.12 + x, 0.30 + y, -8.0f);
				glVertex3f(-0.24 + x, 0.40 + y, -8.0f);
				glVertex3f(0.24 + x, 0.40 + y, -8.0f);
			glEnd();

			glBegin(GL_QUADS);
				glVertex3f(-0.24 + x, 0.40 + y, -8.0f);
				glVertex3f(0.24 + x, 0.40 + y, -8.0f);
				glVertex3f(0.24 + x, 0.54 + y, -8.0f);
				glVertex3f(-0.24 + x, 0.54 + y, -8.0f);
			glEnd();

			for (int i = 0; i < 3; i++) {
				glBegin(GL_QUADS);
					glVertex3f(-0.12 + x + i*0.18, 0.68 + y, -8.0f);
					glVertex3f(-0.24 + x + i*0.18, 0.68 + y, -8.0f);
					glVertex3f(-0.24 + x + i*0.18, 0.54 + y, -8.0f);
					glVertex3f(-0.12 + x + i*0.18, 0.54 + y, -8.0f);
				glEnd();

			}
			break;
		case 'k':
			//knight
			glBegin(GL_QUADS);
			glVertex3f(0.12 + x, 0.30 + y, -8.0f);
			glVertex3f(-0.12 + x, 0.30 + y, -8.0f);
			glVertex3f(-0.01 + x, 0.42 + y, -8.0f);
			glVertex3f(0.23 + x, 0.42 + y, -8.0f);
			glEnd();

			glBegin(GL_QUADS);
			glVertex3f(0.23 + x, 0.42 + y, -8.0f);
			glVertex3f(-0.01 + x, 0.42 + y, -8.0f);
			glVertex3f(-0.01 + x, 0.5 + y, -8.0f);
			glVertex3f(0.23 + x, 0.5 + y, -8.0f);
			glEnd();

			glBegin(GL_QUADS);
			glVertex3f(0.23 + x, 0.5 + y, -8.0f);
			glVertex3f(-0.01 + x, 0.5 + y, -8.0f);
			glVertex3f(-0.12 + x, 0.66 + y, -8.0f);
			glVertex3f(0.12 + x, 0.66 + y, -8.0f);
			glEnd();

			glBegin(GL_QUADS);
			glVertex3f(-0.1 + x, 0.631 + y, -8.0f);
			glVertex3f(-0.02 + x, 0.529 + y, -8.0f);
			glVertex3f(-0.12 + x, 0.4199 + y, -8.0f);
			glVertex3f(-0.2 + x, 0.5219 + y, -8.0f);
			glEnd();

			break;
		case 'b':
			//Bishop
			DrawCircle(0 + x, 0.465 + y, 0.193, 1000, -8.0);
			glBegin(GL_TRIANGLES);
			glVertex3f(-0.12 + x, 0.63 + y, -8.0f);
			glVertex3f(0.12 + x, 0.63 + y, -8.0f);
			glVertex3f(x, 0.70 + y, -8.0f);
			glEnd();
			
			if (piece.getSide() == 0) {
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			else {
				glColor3f(1.0f, 1.0f, 1.0f);
			}

			glBegin(GL_QUADS);
			glVertex3f(0.015 + x, 0.40 + y, -8.0f);
			glVertex3f(-0.015 + x, 0.40 + y, -8.0f);
			glVertex3f(-0.015 + x, 0.58 + y, -8.0f);
			glVertex3f(0.015 + x, 0.58 + y, -8.0f);
			glEnd();

			glBegin(GL_QUADS);
			glVertex3f(0.07 + x, 0.50 + y, -8.0f);
			glVertex3f(-0.07 + x, 0.50 + y, -8.0f);
			glVertex3f(-0.07 + x, 0.520 + y, -8.0f);
			glVertex3f(0.07 + x, 0.520 + y, -8.0f);
			glEnd();

			if (piece.getSide() == 0) {
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 0.0f);
			}
			break;

		case 'Q':
			//Queen

			glBegin(GL_QUADS);
			glVertex3f(0.12 + x, 0.30 + y, -8.0f);
			glVertex3f(-0.12 + x, 0.30 + y, -8.0f);
			glVertex3f(-0.24 + x, 0.50 + y, -8.0f);
			glVertex3f(0.24 + x, 0.50 + y, -8.0f);
			glEnd();

			DrawCircle(x, 0.50 + y, 0.15, 1000, -8.0);
			DrawCircle(x, 0.65 + y, 0.045, 1000, -8.0);
			break;

		case 'K':
			//King
			glBegin(GL_QUADS);
			glVertex3f(0.12 + x, 0.30 + y, -8.0f);
			glVertex3f(-0.12 + x, 0.30 + y, -8.0f);
			glVertex3f(-0.24 + x, 0.50 + y, -8.0f);
			glVertex3f(0.24 + x, 0.50 + y, -8.0f);
			glEnd();

			DrawCircle(x, 0.50 + y, 0.1, 1000,-8.0);
			DrawCircle(x - 0.15, 0.50 + y, 0.08, 1000,-8.0);
			DrawCircle(x + 0.15, 0.50 + y, 0.08, 1000,-8.0);



			glBegin(GL_QUADS);
			glVertex3f(0.03 + x, 0.60 + y, -8.0f);
			glVertex3f(-0.03 + x, 0.60 + y, -8.0f);
			glVertex3f(-0.03 + x, 0.7 + y, -8.0f);
			glVertex3f(0.03 + x, 0.7 + y, -8.0f);
			glEnd();

			glBegin(GL_QUADS);
			glVertex3f(0.07 + x, 0.62 + y, -8.0f);
			glVertex3f(-0.07 + x, 0.62 + y, -8.0f);
			glVertex3f(-0.07 + x, 0.67 + y, -8.0f);
			glVertex3f(0.07 + x, 0.67 + y, -8.0f);
			glEnd();

			break;
	}

	drawChessBase(x, y);

	//Switch case for type
	//side to determin color.


}

void Board::drawChessBase(float x, float y) {

	glBegin(GL_QUADS);
		
		glVertex3f(-0.25 + x, 0.09 + y, -8.0f);
		glVertex3f(0.25 + x, 0.09 + y, -8.0f);
		glVertex3f(0.08 + x, 0.25 + y, -8.0f);
		glVertex3f(-0.08 + x, 0.25 + y, -8.0f);
	glEnd();

	//Bottom Base
	glBegin(GL_QUADS);
		glVertex3f(-0.251 + x, 0.0 + y, -8.0f);
		glVertex3f(0.251 + x, 0.0 + y, -8.0f);
		glVertex3f(0.251 + x, 0.08 + y, -8.0f);
		glVertex3f(-0.251 + x, 0.08 + y, -8.0f);
	glEnd();

	//Top Base
	glBegin(GL_QUADS);
		glVertex3f(-0.12 + x, 0.26 + y, -8.0f);
		glVertex3f(0.12 + x, 0.26 + y, -8.0f);
		glVertex3f(0.12 + x, 0.29 + y, -8.0f);
		glVertex3f(-0.12 + x, 0.29 + y, -8.0f);
	glEnd();
}

void Board::DrawCircle(float cx, float cy, float r, int num_segments, float zoom) {
	
	for (int ii = 0; ii < num_segments; ii++) {
		float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		glBegin(GL_LINE_LOOP);
			glVertex3f(cx, cy, zoom);//output vertex 
			glVertex3f(x + cx, y + cy, zoom);//output vertex 
		glEnd();
	}
}

void Board::setUp() {
	//0.689655 x 0.689655 units cell.

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			pieces[j][i] = ChessPiece();
		}
	}

	for (int i = 0; i < 8; i++) {
		boardPositionX[i] = -2.414793 + 0.689655*i;
		boardPositionY[i] = 2.068965517 - 0.689655*i;
	}

	for (int i = 0; i < 8; i++) {
		pieces[i][1] = ChessPiece('p', 1, boardPositionX[i], boardPositionY[1],i,1);

		pieces[i][6] = ChessPiece('p', 0, boardPositionX[i], boardPositionY[6],i,6);
	}

	pieces[0][0] = ChessPiece('r', 1, boardPositionX[0], boardPositionY[0],0,0);
	pieces[7][0] = ChessPiece('r', 1, boardPositionX[7], boardPositionY[0],7,0);
	pieces[1][0] = ChessPiece('k', 1, boardPositionX[1], boardPositionY[0],1,0);
	pieces[6][0] = ChessPiece('k', 1, boardPositionX[6], boardPositionY[0],6,0);
	pieces[2][0] = ChessPiece('b', 1, boardPositionX[2], boardPositionY[0],2,0);
	pieces[5][0] = ChessPiece('b', 1, boardPositionX[5], boardPositionY[0],5,0);
	pieces[3][0] = ChessPiece('Q', 1, boardPositionX[3], boardPositionY[0],3,0);
	pieces[4][0] = ChessPiece('K', 1, boardPositionX[4], boardPositionY[0],4,0);

	pieces[0][7] = ChessPiece('r', 0, boardPositionX[0], boardPositionY[7],0,7);
	pieces[7][7] = ChessPiece('r', 0, boardPositionX[7], boardPositionY[7],7,7);
	pieces[1][7] = ChessPiece('k', 0, boardPositionX[1], boardPositionY[7],1,7);
	pieces[6][7] = ChessPiece('k', 0, boardPositionX[6], boardPositionY[7],6,7);
	pieces[2][7] = ChessPiece('b', 0, boardPositionX[2], boardPositionY[7],2,7);
	pieces[5][7] = ChessPiece('b', 0, boardPositionX[5], boardPositionY[7],5,7);
	pieces[3][7] = ChessPiece('Q', 0, boardPositionX[3], boardPositionY[7],3,7);
	pieces[4][7] = ChessPiece('K', 0, boardPositionX[4], boardPositionY[7],4,7);


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[j][i].getSide() == -1) {
				pieces[j][i].setX(boardPositionX[j]);
				pieces[j][i].setY(boardPositionY[i]);
			}
		}
	}
}

int Board::getSize() {
	return this->size;
}

Node* Board::SaveTheKing(ChessPiece piece) {
	MoveList list = MoveList();

	int x = piece.posX;
	int y = piece.posY;

	ChessPiece temp = piece;
	ChessPiece temp2;

	bool remove = false;
	if (piece.getType() != 'K') {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {

				if (pieces[j][i].Selected && (j != x || i != y)) {
					temp2 = pieces[j][i];

					pieces[j][i] = temp;
					pieces[j][i].posX = j;
					pieces[j][i].posY = i;
					pieces[j][i].setX(temp2.getX());
					pieces[j][i].setY(temp2.getY());

					pieces[x][y] = ChessPiece();
					pieces[x][y].setX(temp.getX());
					pieces[x][y].setY(temp.getY());

					ClearDanger();
					checker.setDangers(pieces);

					if (piece.getSide() == 0) {
						if (pieces[checker.whiteKingX][checker.whiteKingY].dangerWhite) {
							remove = true;
						}
						else {
							blackWin = false;
						}
					}
					else {
						if (pieces[checker.blackKingX][checker.blackKingY].dangerBlack) {
							remove = true;
						}
						else {
							whiteWin = false;
						}
					}

					pieces[j][i] = temp2;
					pieces[j][i].posX = j;
					pieces[j][i].posY = i;
					pieces[j][i].setX(temp2.getX());
					pieces[j][i].setY(temp2.getY());

					pieces[x][y] = temp;
					pieces[x][y].posX = x;
					pieces[x][y].posY = y;
					pieces[x][y].setX(temp.getX());
					pieces[x][y].setY(temp.getY());

					ClearDanger();
					checker.setDangers(pieces);


					if (remove) {
						pieces[j][i].Selected = false;
						remove = false;
					}
					else {
						list.add(Move(j, i, x, y, temp.getType()), 0);
					}
				}
			}

		}
	}
	else {
		if (x + 1 < 8 && (pieces[x + 1][y].getSide() != pieces[x][y].getSide() || !pieces[x + 1][y].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x + 1][y].dangerBlack) {
				list.add(Move(x + 1, y, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x + 1][y].dangerWhite) {
				list.add(Move(x + 1, y, x, y, 'K'), 0);
			}
		}
		if (x - 1 >= 0 && (pieces[x - 1][y].getSide() != pieces[x][y].getSide() || !pieces[x - 1][y].onBoard)) {

			if (pieces[x][y].getSide() == 1 && !pieces[x - 1][y].dangerBlack) {
				list.add(Move(x - 1, y, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x - 1][y].dangerWhite) {
				list.add(Move(x - 1, y, x, y, 'K'), 0);
			}
		}
		if (x - 1 >= 0 && y - 1 >= 0 && (pieces[x - 1][y - 1].getSide() != pieces[x][y].getSide() || !pieces[x - 1][y - 1].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x - 1][y - 1].dangerBlack) {
				list.add(Move(x - 1, y - 1, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x - 1][y - 1].dangerWhite) {
				list.add(Move(x - 1, y - 1, x, y, 'K'), 0);
			}

		}
		if (x - 1 >= 0 && y + 1 < 8 && (pieces[x - 1][y + 1].getSide() != pieces[x][y].getSide() || !pieces[x - 1][y + 1].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x - 1][y + 1].dangerBlack) {
				list.add(Move(x - 1, y + 1, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x - 1][y + 1].dangerWhite) {
				list.add(Move(x - 1, y + 1, x, y, 'K'), 0);
			}
		}
		if (x + 1 < 8 && y + 1 < 8 && (pieces[x + 1][y + 1].getSide() != pieces[x][y].getSide() || !pieces[x + 1][y + 1].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x + 1][y + 1].dangerBlack) {
				list.add(Move(x + 1, y + 1, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x + 1][y + 1].dangerWhite) {
				list.add(Move(x + 1, y + 1, x, y, 'K'), 0);
			}
		}
		if (x + 1 < 8 && y - 1 >= 0 && (pieces[x + 1][y + 1].getSide() != pieces[x][y].getSide()|| !pieces[x+1][y-1].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x + 1][y - 1].dangerBlack) {
				list.add(Move(x + 1, y - 1, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x + 1][y - 1].dangerWhite) {
				list.add(Move(x + 1, y - 1, x, y, 'K'), 0);
			}
		}
		if (y + 1 < 8 && (pieces[x][y + 1].getSide() != pieces[x][y].getSide() || !pieces[x][y+1].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x][y + 1].dangerBlack) {
				list.add(Move(x , y + 1, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x][y + 1].dangerWhite) {
				list.add(Move(x, y + 1, x, y, 'K'), 0);
			}
		}

		if (y - 1 >= 0 && (pieces[x][y - 1].getSide() != pieces[x][y].getSide() || !pieces[x][y-1].onBoard)) {
			if (pieces[x][y].getSide() == 1 && !pieces[x][y - 1].dangerBlack) {
				list.add(Move(x, y - 1, x, y, 'K'), 0);
			}
			if (pieces[x][y].getSide() == 0 && !pieces[x][y - 1].dangerWhite) {
				list.add(Move(x, y - 1, x, y, 'K'), 0);
			}
		}

	}
	
	return list.getHead();
}

void Board::CheckGameStatus() {
	if (!blackWin && !whiteWin) {
		int x;
		int y;
		int side = -1;

		if (pieces[checker.blackKingX][checker.blackKingY].dangerBlack) {
			side = 1;
			x = checker.blackKingX;
			y = checker.blackKingY;
			whiteWin = true;
		}
		else if (pieces[checker.whiteKingX][checker.whiteKingY].dangerWhite) {
			side = 0;
			x = checker.whiteKingX;
			y = checker.whiteKingY;
			blackWin = true;
		}

		if (side != -1) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (pieces[j][i].getSide() == side) {
						checker.Hightlight(pieces, j, i, false);
						SaveTheKing(pieces[j][i]);
						checker.resetHighlight(pieces);

						if (side == 1 && !whiteWin) {
							break;
						}
						else if (side == 0 && !blackWin) {
							break;
						}
					}
				}
			}
		}

		if (blackWin || whiteWin) {
			if (blackWin) {
				checker.Hightlight(pieces, checker.whiteKingX, checker.whiteKingY, false);
			}
			else {
				checker.Hightlight(pieces, checker.blackKingX, checker.blackKingY, false);
			}

			if ((x - 1) >= 0 && (y - 1) >= 0 && pieces[x - 1][y - 1].Selected) {
				blackWin = false;
				whiteWin = false;
			}

			if ((x - 1) >= 0 && pieces[x - 1][y].Selected) {
				blackWin = false;
				whiteWin = false;
			}
			if ((x - 1) >= 0 && (y + 1) < 8 && pieces[x - 1][y + 1].Selected) {
				blackWin = false;
				whiteWin = false;
			}
			if ((y - 1) >= 0 && pieces[x][y - 1].Selected) {
				blackWin = false;
				whiteWin = false;
			}
			if ((y + 1)  < 8 && pieces[x][y + 1].Selected) {
				blackWin = false;
				whiteWin = false;
			}
			if ((x + 1) < 8 && (y - 1) >= 0 && pieces[x + 1][y - 1].Selected) {
				blackWin = false;
				whiteWin = false;
			}
			if ((x + 1) < 8 && (y + 1) < 8 && pieces[x + 1][y + 1].Selected) {
				blackWin = false;
				whiteWin = false;
			}
			if ((x + 1) < 8 && pieces[x + 1][y].Selected) {
				blackWin = false;
				whiteWin = false;
			}

			checker.resetHighlight(pieces);
		}
	}

	if (blackWin) {
		cout << "Black Wins" << endl;
		checker.GameOver = true;
		checker.resetHighlight(pieces);
	}
	else if (whiteWin) {
		cout << "White Wins" << endl;
		checker.GameOver = true;
		checker.resetHighlight(pieces);
	}


}

void Board::AIMove() {
	checker.resetHighlight(pieces);


	if (moveCount < 2) {
		nextMove = Opening(currentMove);
	}
	else {
		nextMove = bestMove(1, 1);
	}

	if (checker.GameOver == false) {
		this->current = pieces[nextMove.getCX()][nextMove.getCY()];
		draw();

		//Time delay
		Sleep(250);
		current.move(pieces, nextMove.getX(), nextMove.getY(), boardPositionX, boardPositionY, turn);
	}
}

void Board::ClearDanger() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			pieces[i][j].dangerBlack = false;
			pieces[i][j].dangerWhite = false;
		}
	}
}

void Board::reset() {
	cout << "Restart Game!" << endl;

	ClearDanger();
	checker.resetHighlight(pieces);
	checker.GameOver = false;

	this->start = true;
	this->PieceSeleced = false;
	this->turn = 0;
	this->moveCount = 0;
	blackWin = false;
	whiteWin = false;

	draw();
}

//AI methods
Move Board::Opening(Move start) {
	Move move;
	srand(time(NULL));

	if (moveCount == 0) {

		if (start.getX() == 4 && start.getY() == 4 && pieces[4][4].getType() == 'p') {
			firstMove = Move(4, 4, 4, 6, pieces[4][4].getType());


			opening = (rand() % 6 + 1);

			switch (opening) {
			case 1:
				move = Move(2, 3, 2, 1, pieces[2][1].getType());
				break;
			case 2:
				move = Move(4, 2, 4, 1, pieces[4][1].getType());
				break;
			case 3:
				move = Move(2, 2, 2, 1, pieces[2][1].getType());
				break;
			case 4:
				move = Move(5, 2, 6, 0, pieces[6][0].getType());
				break;
			case 5:
				move = Move(4, 3, 4, 1, pieces[4][1].getType());
				break;
			case 6:
				move = Move(3, 2, 3, 1, pieces[3][1].getType());
				break;
			}
		}
		else if (start.getX() == 3 && start.getY() == 4 && pieces[3][4].getType() == 'p') {
			firstMove = Move(3, 4, 3, 6, pieces[3][4].getType());

			opening = (rand() % 2 + 1);

			switch (opening) {
			case 1:
				move = Move(3, 3, 3, 1, pieces[3][1].getType());
				break;
			case 2:
				move = Move(5, 2, 6, 0, pieces[6][0].getType());
				break;
			}
		}
		else if (start.getX() == 5 && start.getY() == 5 && pieces[5][5].getType() == 'k') {
			firstMove = Move(5, 5, 6, 7, pieces[5][5].getType());

			opening = (rand() % 4 + 1);

			switch (opening) {
			case 1:
				move = Move(5, 2, 6, 0, pieces[6][0].getType());
				break;
			case 2:
				move = Move(3, 3, 3, 1, pieces[3][1].getType());
				break;
			case 3:
				move = Move(2, 3, 2, 1, pieces[2][1].getType());
				break;
			case 4:
				move = Move(6, 2, 6, 1, pieces[6][1].getType());
				break;
			}
		}
		else {
			firstMove = Move(0, 0, 0, 0, 'a');

			int temp;
			bool pass = false;

			while (!pass) {
				temp = (rand() % 3 + 1);

				switch (temp) {
				case 1:
					if (!pieces[5][2].dangerBlack) {
						move = Move(5, 2, 6, 0, pieces[6][0].getType());
						pass = true;
					}
					else {
						if (pieces[5][2].dangerWhite) {
							move = Move(5, 2, 6, 0, pieces[6][0].getType());
							pass = true;
						}
					}


					break;
				case 2:
					if (!pieces[4][3].dangerBlack) {
						move = Move(4, 3, 4, 1, pieces[4][1].getType());
						pass = true;
					}
					else {
						if (pieces[4][3].dangerWhite) {
							move = Move(4, 3, 4, 1, pieces[4][1].getType());
							pass = true;
						}
					}
					break;
				case 3:
					if (!pieces[3][3].dangerBlack) {
						move = Move(3, 3, 3, 1, pieces[3][1].getType());
						pass = true;
					}
					else {
						if (pieces[3][3].dangerWhite) {
							move = Move(3, 3, 3, 1, pieces[3][1].getType());
							pass = true;
						}
					}
					break;
				}
			}
		}
	}

	if (moveCount == 1) {
		if (firstMove.getX() == 4 && firstMove.getY() == 4 && firstMove.getType() == 'p') {
			switch (opening) {
			case 1:
				if (rand() % 2 == 0) {
					move = Move(2, 2, 1, 0, pieces[1][0].getType());
				}
				else {
					move = Move(3, 2, 3, 1, pieces[3][1].getType());
				}

				break;
			case 2:
				move = Move(3, 3, 3, 1, pieces[3][1].getType());
				break;
			case 3:
				move = Move(3, 3, 3, 1, pieces[3][1].getType());
				break;
			case 4:
				if (pieces[4][3].onBoard) {
					move = Move(3, 3, 5, 2, pieces[5][2].getType());
				}
				else {
					move = Move(4, 3, 4, 1, pieces[4][1].getType());
				}
				break;
			case 5:
				move = Move(2, 2, 1, 0, pieces[1][0].getType());
				break;
			case 6:
				move = Move(5, 2, 6, 0, pieces[6][0].getType());
				break;
			}
		}
		else if (firstMove.getX() == 3 && firstMove.getY() == 4 && firstMove.getType() == 'p') {
			switch (opening) {
			case 1:
				if (rand() % 2 == 0) {
					move = Move(2, 2, 2, 1, pieces[2][1].getType());
				}
				else {
					move = Move(4, 2, 4, 1, pieces[4][1].getType());
				}

				break;
			case 2:
				if (rand() % 2 == 0) {
					move = Move(6, 2, 6, 1, pieces[6][1].getType());
				}
				else {
					move = Move(4, 2, 4, 1, pieces[4][1].getType());
				}
				break;
			}
		}
		else if ((firstMove.getX() == 5 && firstMove.getY() == 5) && firstMove.getType() == 'k') {
			switch (opening) {
			case 1:
				if (rand() % 2 == 0) {
					move = Move(6, 2, 6, 1, pieces[6][1].getType());
				}
				else {
					move = Move(4, 2, 4, 1, pieces[4][1].getType());
				}
				break;
			case 2:
				if (rand() % 2 == 0) {
					move = Move(5, 2, 6, 0, pieces[6][0].getType());
				}
				else {
					move = Move(2, 2, 2, 1, pieces[2][1].getType());
				}
				break;
			case 3:
				if (rand() % 2 == 0) {
					move = Move(4, 2, 4, 1, pieces[4][1].getType());
				}
				else {
					move = Move(2, 2, 1, 0, pieces[1][0].getType());
				}
				break;
			case 4:
				if (rand() % 2 == 0) {
					move = Move(6, 1, 5, 0, pieces[5][0].getType());
				}
				else {
					move = Move(5, 2, 6, 0, pieces[6][0].getType());
				}
				break;
			}
		}
		else {
			move = bestMove(1, 1); //edit to increase AI level.
		}
	}

	return move;
}


//Note:
/*
make it consider all possible moves and use another recurive function to 
add value to each move.


*/
Move Board::bestMove(int level, int side) {
	Node* head;
	int count = 0, max = -100, score = 0;
	MoveList list = MoveList();
	Move best, temp;

	int K = 100, Q = 20, k = 10, b = 10, r = 9, p = 2;
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[j][i].onBoard) {
				if (pieces[j][i].getSide() == side) {

					checker.Hightlight(pieces, j, i, false);

					head = SaveTheKing(pieces[j][i]);

					checker.setDangers(pieces);
					while (head != NULL) {
						temp = head->getMove();

						if (side == 1) {
							if (moveCount < 4) {
								if (pieces[j][i].getType() == 'p' && j >= 1 && j <= 6) {
									if (pieces[j][i].hasMoved == false) {
										score += 1;
									}
								}
							}

							
							if (pieces[j][i].getType() == 'p' && pieces[temp.getX()][temp.getY()].dangerBlack && pieces[temp.getX()][temp.getY()].dangerWhite) {
								score += 3;
							}
							else if (pieces[j][i].getType() == 'p' && pieces[j][i].dangerBlack && pieces[j][i].dangerWhite) {
								score -= 2;
							}
							
							if (pieces[j][i].dangerBlack) {

								if (!pieces[temp.getX()][temp.getY()].dangerBlack) {
									switch (pieces[j][i].getType()) {
									case 'K':
										score += K;
										break;
									case 'Q':
										score += Q;
										break;
									case 'k':
										score += k;
										break;
									case 'b':
										score += b;
										break;
									case 'r':
										score += r;
										break;
									case 'p':
										score += p;
										break;
									}
								}
							}

							if (pieces[temp.getX()][temp.getY()].dangerBlack) {
								switch (pieces[j][i].getType()) {
								case 'K':
									score -= K / 2;
									break;
								case 'Q':
									score -= Q / 2;
									break;
								case 'k':
									score -= k / 2;
									break;
								case 'b':
									score -= b / 2;
									break;
								case 'r':
									score -= r / 2;
									break;
								case 'p':
									score -= p / 2;
									break;
								}
							}

							if (pieces[temp.getX()][temp.getY()].onBoard) {
								switch (pieces[temp.getX()][temp.getY()].getType()) {
								case 'K':
									score += K;
									break;
								case 'Q':
									score += Q;
									break;
								case 'k':
									score += k;
									break;
								case 'b':
									score += b;
									break;
								case 'r':
									score += r;
									break;
								case 'p':
									score += p;
									break;
								}
							}
							
							
							if (score >= max) {
								list.add(temp, score);
								max = score;
								count++;
							}

							score = 0;
						}

						Node* temp2 = head;
						head = head->getNext();

						delete temp2;

					}

					checker.resetHighlight(pieces);
				}
			}
		}
	}
	

	while(list.getHead() != NULL){
		if (list.peek(count) != max) {
			list.remove(count);
			count--;
		}
		else {
			break;
		}
	}

	head = list.getHead();
	
	if (count > 0) {
		best = list.returnMove(rand() % count + 1);
	}
	else {
		whiteWin = true;
		checker.GameOver = true;
	}
	

	//recursive function
	//move
	//check the importantance with recurvice function, and add points
	// move back

	return best;
}