#include "board.h"

//Base Board of the game
Board::Board() {
	this->start = true;
	this->PieceSeleced = false;
	checker = LegalMove();
	this->turn = 0;
	this->moveCount = 0;
}

//Draw the board on OpenGL
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

//Restricted the size of the game to a fixed size.
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

//Control any mouse input.
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

			//cout << "Danger: " << pieces[cx][cy].dangerWhite << " " << pieces[cx][cy].dangerBlack << endl;

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
							
							checker.resetHighlight(pieces);
							current.move(pieces, cx, cy, boardPositionX, boardPositionY, turn);

							turn = !turn;

							CheckGameStatus();

							if (AIGames && !blackWin && ! whiteWin) {
								draw();

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

//Set up all the chess pieces on the board.
void Board::setBoard() {

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[i][j].onBoard == true) {
				setPiece(pieces[i][j], boardPositionX[i], boardPositionY[j]);
			}

			pieces[i][j].dangerBlack = false;
			pieces[i][j].dangerWhite = false;
		}
	}
}

//Set up each chess piece on the board.
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
}

//Draw the base of the chess piece.
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
		boardPositionX[i] = -2.414793 + 0.689655*i;
		boardPositionY[i] = 2.068965517 - 0.689655*i;
	}

	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			pieces[j][i] = ChessPiece();
		}
	}

	for (int i = 0; i < 8; i++) {
		pieces[i][1] = ChessPiece('p', 1,i,1);

		pieces[i][6] = ChessPiece('p', 0,i,6);
	}

	pieces[0][0] = ChessPiece('r', 1,0,0);
	pieces[7][0] = ChessPiece('r', 1,7,0);
	pieces[1][0] = ChessPiece('k', 1,1,0);
	pieces[6][0] = ChessPiece('k', 1,6,0);
	pieces[2][0] = ChessPiece('b', 1,2,0);
	pieces[5][0] = ChessPiece('b', 1,5,0);
	pieces[3][0] = ChessPiece('Q', 1, 3,0);
	pieces[4][0] = ChessPiece('K', 1,4,0);

	pieces[0][7] = ChessPiece('r', 0, 0,7);
	pieces[7][7] = ChessPiece('r', 0, 7,7);
	pieces[1][7] = ChessPiece('k', 0, 1,7);
	pieces[6][7] = ChessPiece('k', 0, 6,7);
	pieces[2][7] = ChessPiece('b', 0,2,7);
	pieces[5][7] = ChessPiece('b', 0,5,7);
	pieces[3][7] = ChessPiece('Q', 0,3,7);
	pieces[4][7] = ChessPiece('K', 0,4,7);
}

int Board::getSize() {
	return this->size;
}

//Return a node containina all possible moves allowd by the piece without putting the king in danger.
Node* Board::SaveTheKing(ChessPiece piece) {
	MoveList list = MoveList();

	int x = piece.posX;
	int y = piece.posY;

	ChessPiece temp = piece;
	ChessPiece temp2;

	bool remove = false;
	bool king;
	int side;

	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			if (pieces[j][i].Selected && (j != x || i != y)) {

				temp2 = pieces[j][i];
				side = piece.getSide();

				if (checker.whiteKingX == x && checker.whiteKingY == y) {
					checker.moveWhiteKing(j, i);
					king = true;
				}
				else if (checker.blackKingX == x && checker.blackKingY == y) {
					checker.moveBlackKing(j, i);

					king = true;
				}
				else {
					king = false;
				}

				pieces[j][i] = piece;
				pieces[x][y] = ChessPiece();

				ClearDanger();
				checker.setDangers(pieces);

				//Check if king is in danger after the swap
				if (side == 0) {
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

				pieces[x][y] = piece;

				if (king) {
					if (side == 0) {
						checker.moveWhiteKing(x, y);
					}
					else {
						checker.moveBlackKing(x, y);
					}
				}


				ClearDanger();
				checker.setDangers(pieces);


				if (remove) {
					pieces[j][i].Selected = false;
					remove = false;
				}
				else {
					if (king) {
						if (side == 0 && piece.dangerWhite || side == 1 && piece.dangerBlack) {
							list.add(Move(j, i, x, y, piece.getType()));
						}
					}
					else {
						list.add(Move(j, i, x, y, piece.getType()));
					}
					
				}
			}
		}

	}
	
	return list.getHead();
}

//Check if the game is over.
void Board::CheckGameStatus() {
	if (!blackWin && !whiteWin) {
		int side = -1;
		Node* head = NULL;
		ChessPiece current;

		if (pieces[checker.blackKingX][checker.blackKingY].dangerBlack) {
			side = 1;
			whiteWin = true;
		}
		else if (pieces[checker.whiteKingX][checker.whiteKingY].dangerWhite) {
			side = 0;
			blackWin = true;
		}

		checker.resetHighlight(pieces);

		if (side != -1) {

			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					current = pieces[j][i];

					if (current.onBoard && current.getSide() == side) {
						checker.Hightlight(pieces, j, i, false);
						head = SaveTheKing(current);
						checker.resetHighlight(pieces);

						if (side == 1) {
							if (head != NULL) {
								whiteWin = false;
								break;
							}
						}
						else {
							if (head != NULL) {
								blackWin = false;
								break;
							}
						}

					}
				}
			}
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


//Clear danger area on the board.
void Board::ClearDanger() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			pieces[i][j].dangerBlack = false;
			pieces[i][j].dangerWhite = false;
		}
	}
}

//Reset the game
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
void Board::AIMove() {
	checker.resetHighlight(pieces);

	if (moveCount < 2) {
		nextMove = Opening(currentMove);
	}
	else {
		nextMove = bestMove(AILevel + 1, 1);
	}

	if (checker.GameOver == false) {
		this->current = pieces[nextMove.getCX()][nextMove.getCY()];
		draw();

		//Time delay
		//Sleep(250);

		current.move(pieces, nextMove.getX(), nextMove.getY(), boardPositionX, boardPositionY, turn);
		if (current.getType() == 'K') {
			if (current.getSide() == 0) {
				checker.moveWhiteKing(nextMove.getX(), nextMove.getY());
			}
			else {
				checker.moveBlackKing(nextMove.getX(), nextMove.getY());
			}
		}
	}

	turn = !turn;
}

//Pre-set opening moves for the AI
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
			move = bestMove(AILevel + 1, 1); //edit to increase AI level.
		}
	}

	return move;
}

//Return the move with the best value.
Move Board::bestMove(int level, int side) {

	MoveList move = AllMove(side);
	Node* head = move.getHead();

	ChessPiece current, replaced;
	Move best, temp;
	bool king = false;

	MoveList list = MoveList();

	int max = INT_MIN;

	int num, score, x, y, cx, cy;


	while (head != NULL) {
		//Move piece

		temp = head->getMove();
		cx = temp.getCX();
		cy = temp.getCY();
		x = temp.getX();
		y = temp.getY();
		current = pieces[cx][cy];
		replaced = pieces[x][y];

		pieces[x][y] = current;
		pieces[x][y].posX = x;
		pieces[x][y].posY = y;

		pieces[cx][cy] = ChessPiece();

		if (checker.blackKingX == cx && checker.blackKingY == cy) {
			checker.moveBlackKing(x, y);

			king = true;
		}
		else {
			king = false;
		}

		score = -ScoreMove(level - 1, !side, max);

		if (score >= max) {
			if (score > max) {
				list.clear();
				max = score;
			}

			list.add(head->getMove());
		}

		pieces[x][y] = replaced;

		pieces[cx][cy] = current;
		pieces[cx][cy].posX = cx;
		pieces[cx][cy].posY = cy;
		
		if (king) {
			checker.moveBlackKing(cx, cy);
		}

		head = head->getNext();
	}

	if (list.size() > 0) {
		num = (rand() % list.size()) + 1;

		best = list.returnMove(num);

		list.clear();
	}
	else {
		whiteWin = true;
		checker.GameOver = true;
	}

	move.clear();

	return best;
}

//Finding the move with the best value.
int Board::ScoreMove(int depth, int side, int points) {

	Node* head;
	Node* temp2;
	ChessPiece current, replaced;
	Move temp;
	int max = INT_MIN;
	bool king = false;
	bool exit = false;
	int score, x, y;

	if (depth == 0) {
		return calculateScore(side);
	}
	else {
		checker.resetHighlight(pieces);

		//Center Board
		for (int j = 2; j < 6; j++) {
			for (int i = 0; i < 8; i++) {
				current = pieces[i][j];

				if (current.onBoard && current.getSide() == side) {

					checker.Hightlight(pieces, i, j, false);
					head = SaveTheKing(current);
					checker.resetHighlight(pieces);

					while (head != NULL) {
						//Move piece

						temp2 = head;
						temp = head->getMove();

						x = temp.getX();
						y = temp.getY();
						replaced = pieces[x][y];

						pieces[x][y] = current;
						pieces[x][y].posX = x;
						pieces[x][y].posY = y;

						pieces[i][j] = ChessPiece();

						if (checker.whiteKingX == i && checker.whiteKingY == j) {
							checker.moveWhiteKing(x, y);
							king = true;
						}
						else if (checker.blackKingX == i && checker.blackKingY == j) {
							checker.moveBlackKing(x, y);

							king = true;
						}
						else {
							king = false;
						}

						if (points != INT_MIN) {
							if (max != INT_MIN) {
								score = -ScoreMove(depth - 1, !side, max);
							}
							else {
								score = -ScoreMove(depth - 1, !side, -points);
							}	
						}
						else {
							score = -ScoreMove(depth - 1, !side, points);
						}

						if (-1 * score >= points) {

							if (score > max) {
								max = score;
							}
						}
						else {
							exit = true;
						}

						pieces[x][y] = replaced;

						pieces[i][j] = current;
						pieces[i][j].posX = i;
						pieces[i][j].posY = j;

						if (king) {
							if (side == 0) {
								checker.moveWhiteKing(i, j);
							}
							else {
								checker.moveBlackKing(i, j);
							}
						}


						if (!exit) {
							head = head->getNext();
							delete temp2;
						}
						else {
							while (head != NULL) {
								head = head->getNext();

								delete temp2;

								temp2 = head;
							}

							return score;
						}
					}


				}
			}
		}

		//Side Board
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < 8; i++) {
				current = pieces[i][j];

				if (current.onBoard && current.getSide() == side) {

					checker.Hightlight(pieces, i, j, false);
					head = SaveTheKing(current);
					checker.resetHighlight(pieces);

					while (head != NULL) {
						//Move piece

						temp2 = head;
						temp = head->getMove();

						x = temp.getX();
						y = temp.getY();
						replaced = pieces[x][y];

						pieces[x][y] = current;
						pieces[x][y].posX = x;
						pieces[x][y].posY = y;

						pieces[i][j] = ChessPiece();

						if (checker.whiteKingX == i && checker.whiteKingY == j) {
							checker.moveWhiteKing(x, y);
							king = true;
						}
						else if (checker.blackKingX == i && checker.blackKingY == j) {
							checker.moveBlackKing(x, y);

							king = true;
						}
						else {
							king = false;
						}



						if (points != INT_MIN) {
							if (max != INT_MIN) {
								score = -ScoreMove(depth - 1, !side, max);
							}
							else {
								score = -ScoreMove(depth - 1, !side, -points);
							}
						}
						else {
							score = -ScoreMove(depth - 1, !side, points);
						}	

						if (-1 * score >= points) {

							if (score > max) {
								max = score;
							}
						}
						else {
							exit = true;
						}

						pieces[x][y] = replaced;

						pieces[i][j] = current;
						pieces[i][j].posX = i;
						pieces[i][j].posY = j;

						if (king) {
							if (side == 0) {
								checker.moveWhiteKing(i, j);
							}
							else {
								checker.moveBlackKing(i, j);
							}
						}


						if (!exit) {
							head = head->getNext();
							delete temp2;
						}
						else {
							while (head != NULL) {
								head = head->getNext();

								delete temp2;

								temp2 = head;
							}

							return score;
						}
					}


				}
			}

			if (j == 1) {
				j = 5;
			}
		}
		
	}

	return max;
}

//Return the value of the board at the current state.
int Board::calculateScore(int side) {
	wK = bK = wQ = bQ = wR = bR = wN = bN = wB = bB = wP = bP = 0;

	int currentSide;

	int score;
	unsigned int wMove = 0, bMove = 0;
	MoveList list;

	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			if (pieces[i][j].onBoard) {

				currentSide = pieces[i][j].getSide();

				switch (pieces[i][j].getType()) {
				case 'K':
					if (currentSide == 0) {
						wK++;
					}
					else {
						bK++;
					}
					break;
				case 'Q':
					if (currentSide == 0) {
						wQ++;
					}
					else {
						bQ++;
					}
					break;
				case 'r':
					if (currentSide == 0) {
						wR++;
					}
					else {
						bR++;
					}
					break;
				case 'k':
					if (currentSide == 0) {
						wN++;
					}
					else {
						bN++;
					}
					break;
				case 'b':
					if (currentSide == 0) {
						wB++;
					}
					else {
						bB++;
					}
					break;
				case 'p':
					if (currentSide == 0) {
						wP++;
					}
					else {
						bP++;
					}
					break;
				}

			}
		}
	}

	list = AllMove(0);

	wMove = list.size();

	list.clear();

	list = AllMove(1);

	bMove = list.size();

	list.clear();

	score = K*(wK - bK) + Q*(wQ - bQ) + r*(wR - bR) + k*(wN - bN) + b*(wB - bB) + p*(wP - bP)
		+ mobilityWeight*(wMove - bMove);
	/*
		cout << "K: " << wK << " " << bK << endl;
		cout << "Q: " << wQ << " " << bQ << endl;
		cout << "R: " << wR << " " << bR << endl;
		cout << "N: " << wN << " " << bN << endl;
		cout << "B: " << wB << " " << bB << endl;
		cout << "P: " << wP << " " << bP << endl;
		cout << "Score " << score << " " << wMove << " " << bMove << endl;
	*/

	if (side == 0) {
		return score;
	}
	else{
		return score*-1;
	}
}

//Return all possible move by one of the side.
MoveList Board::AllMove(int side) {
	Node* head = NULL;
	Node* temp;

	MoveList list = MoveList();

	checker.resetHighlight(pieces);

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (pieces[j][i].onBoard) {
				if (pieces[j][i].getSide() == side) {
					checker.Hightlight(pieces, j, i, false);

					head = SaveTheKing(pieces[j][i]);
					temp = head;

					while (head != NULL) {
						list.add(head->getMove());

						head = head->getNext();

						delete temp;

						temp = head;
						
					}

					checker.resetHighlight(pieces);
				}
			}
		}
	}

	return list;
}	