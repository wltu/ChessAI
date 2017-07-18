#include "LegalMove.h"
#include <iostream>
using namespace std;

LegalMove::LegalMove() {
}

//Hilight light possible move for the current chess piece.
void LegalMove::Hightlight(ChessPiece board[8][8], int x, int y, bool danger) {
	if (!GameOver) {

		if (!danger) {
			board[x][y].Selected = true;
		}

		int side = board[x][y].getSide();

		int knightMoveX[4] = { -1,1,-2,2 };
		int knightMoveY[4] = { -2,2,-1,1 };

		switch (board[x][y].getType()) {
		case'p':
			if (side == 0) {
				i = 1;
			}
			else {
				i = -1;
			}

			if (y - i >= 0 && y - i < 8) {
				if (!danger && !board[x][y - i].onBoard) {
					action(board, x, y, x, y - i, false, side);

					if (!board[x][y - 2 * i].onBoard && !board[x][y].hasMoved) {
						action(board, x, y, x, y - 2 * i, false, side);
					}
				}

				if (x + 1 < 8 && board[x + 1][y - i].onBoard && board[x + 1][y - i].getSide() != side || danger) {
					action(board, x, y, x + 1, y - i, danger, side);
				}
				if (x - 1 >= 0 && board[x - 1][y - i].onBoard && board[x - 1][y - i].getSide() != side || danger) {
					action(board, x, y, x - 1, y - i, danger, side);
				}
			}

			break;

		case 'r':
			RookMove(board, x, y, 8, side, danger);

			break;

		case 'k':
			for (int a = 0; a < 4; a++) {
				if (a < 2) {
					for (int b = 0; b < 2; b++) {
						if ((y + knightMoveY[b]) >= 0 && (y + knightMoveY[b]) < 8) {
							if ((x + knightMoveX[a]) >= 0 && (x + knightMoveX[a]) < 8) {
								if (!board[x + knightMoveX[a]][y + knightMoveY[b]].onBoard || danger) {
									action(board, x, y, x + knightMoveX[a], y + knightMoveY[b], danger, side);
								}
								else if (board[x + knightMoveX[a]][y + knightMoveY[b]].onBoard && board[x + knightMoveX[a]][y + knightMoveY[b]].getSide() != side || danger) {
									action(board, x, y, x + knightMoveX[a], y + knightMoveY[b], danger, side);
								}
							}
						}
					}
				}
				else {
					for (int b = 2; b < 4; b++) {
						if ((y + knightMoveY[b]) >= 0 && (y + knightMoveY[b]) < 8) {
							if ((x + knightMoveX[a]) >= 0 && (x + knightMoveX[a]) < 8) {
								if (!board[x + knightMoveX[a]][y + knightMoveY[b]].onBoard || danger) {
									action(board, x, y, x + knightMoveX[a], y + knightMoveY[b], danger, side);
								}
								else if (board[x + knightMoveX[a]][y + knightMoveY[b]].onBoard && board[x + knightMoveX[a]][y + knightMoveY[b]].getSide() != side || danger) {
									action(board, x, y, x + knightMoveX[a], y + knightMoveY[b], danger, side);
								}
							}
						}
					}
				}
			}
			break;

		case 'b':
			BishopMove(board, x, y, 8, side, danger);
			break;
		case 'Q':
			RookMove(board, x, y, 8, side, danger);
			BishopMove(board, x, y, 8, side, danger);
			break;
		case 'K':
			RookMove(board, x, y, 1, side, danger);
			BishopMove(board, x, y, 1, side, danger);
				
			if (!danger) {
				Castling(board, x, y);

			}
			break;
		}
	}
}

//Show possible rook moves.
void LegalMove::RookMove(ChessPiece board[8][8], int x, int y, int num, int side, bool danger){
	i = 1;

	while ((y - i) >= 0) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x, y - 1, danger, side);
			}
			if (side == 0 && board[x][y - i].dangerWhite || side == 1 && board[x][y - i].dangerBlack) {
				break;
			}
		}

		if (!board[x][y - i].onBoard) {
			action(board, x, y, x, y - i, danger, side);
			
		}else if (board[x][y - i].getSide() != side || danger) {
			action(board, x, y, x, y - i, danger, side);
			break;
			
		}
		else if (board[x][y - i].getSide() == side) {
			break;
		}
		else {
			break;
		}
		
		i++;
	}

	i = 1;

	while ((y + i) < 8) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x, y + 1, danger, side);
			}
			if (side == 0 && board[x][y + i].dangerWhite || side == 1 && board[x][y + i].dangerBlack) {
				break;
			}
		}

		if (!board[x][y + i].onBoard) {
			action(board, x, y, x, y + i, danger, side);
		}else if (board[x][y + i].getSide() != side || danger) {
			action(board, x, y, x, y + i, danger, side);
			
			break;
			
		}
		else if (board[x][y + i].getSide() == side) {
			break;
		}
		else {
			break;
		}
		
		i++;
	}

	i = 1;

	while ((x - i) >= 0) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x - 1, y, danger, side);
			}
			if (side == 0 && board[x - i][y].dangerWhite || side == 1 && board[x - i][y].dangerBlack) {
				break;
			}
		}

		if (!board[x - i][y].onBoard) {
			action(board, x, y, x - i, y, danger, side);
		}
		else if (board[x - i][y].getSide() != side || danger) {
			action(board, x, y, x - i, y, danger, side);
			
			break;
			
		}
		else if (board[x - i][y].getSide() == side) {
			break;
		}
		else {
			break;
		}

		i++;
	}

	i = 1;
	while ((x + i) < 8) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x + 1, y, danger, side);
			}
			if (side == 0 && board[x + i][y].dangerWhite || side == 1 && board[x + i][y].dangerBlack) {
				break;
			}
		}

		if (!board[x + i][y].onBoard) {
			action(board, x, y, x + i, y, danger, side);
		}else if(board[x + i][y].getSide() != side || danger) {
			action(board, x, y, x + i, y, danger, side);
			
			break;
			
		}
		else if (board[x + i][y].getSide() == side) {
			break;
		}
		else {
			break;
		}
		
		i++;
	}

}

//Show possible bishop moves.
void LegalMove::BishopMove(ChessPiece board[8][8], int x, int y, int num, int side, bool danger) {
	i = 1;

	while ((x - i) >= 0 && (y - i) >= 0) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x - 1, y - 1, danger, side);
			}
			if (side == 0 && board[x - i][y - i].dangerWhite || side == 1 && board[x - i][y - i].dangerBlack) {
				break;
			}
		}

		if (!board[x - i][y - i].onBoard) {
			action(board, x, y, x - i, y - i, danger, side);
		}else if(board[x - i][y -i].getSide() != side || danger){
			action(board, x, y, x - i, y - i, danger, side);
			
			break;
			
		}
		else if (board[x - i][y - i].getSide() == side) {
			break;
		}
		else {
			break;
		}

		i++;
	}

	i = 1;

	while ((x + i) < 8 && (y + i) < 8) {

		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x + 1, y + 1, danger, side);
			}
			if (side == 0 && board[x + i][y + i].dangerWhite || side == 1 && board[x + i][y + i].dangerBlack) {
				break;
			}
		}

		if (!board[x + i][y + i].onBoard) {

			action(board, x, y, x + i, y + i, danger, side);
		}else if (board[x + i][y + i].getSide() != side || danger) {
			action(board, x, y, x + i, y + i, danger, side);
			break;

		}
		else if (board[x + i][y + i].getSide() == side) {
			break;
		}
		else {
			break;
		}

		i++;
	}

	i = 1;

	while ((x - i) >= 0 && (y + i) <8) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x - 1, y + 1, danger, side);
			}
			if (side == 0 && board[x - i][y + i].dangerWhite || side == 1 && board[x - i][y + i].dangerBlack) {
				break;
			}
		}

		if (!board[x - i][y + i].onBoard) {
			action(board, x, y, x - i, y + i, danger, side);
		}else if (board[x - i][y + i].getSide() != side || danger) {
			action(board, x, y, x - i, y + i, danger, side);
			break;
		
		}
		else if (board[x - i][y + i].getSide() == side) {
			break;
		}
		else {
			break;
		}

		i++;
	}

	i = 1;

	while ((x + i) < 8 && (y - i) >= 0) {
		if (i > num) {
			break;
		}

		if (board[x][y].getType() == 'K') {
			if (danger) {
				action(board, x, y, x + 1, y - 1, danger, side);
			}
			if (side == 0 && board[x + i][y - i].dangerWhite || side == 1 && board[x + i][y - i].dangerBlack) {
				break;
			}
		}

		if (!board[x + i][y - i].onBoard) {
			action(board, x, y, x + i, y - i, danger, side);
			
		}else if (board[x + i][y - i].getSide() != side || danger) {
			action(board, x, y, x + i, y - i, danger, side);
			
			break;
		}else if (board[x + i][y - i].getSide() == side) {
			break;
		}
		else {
			break;
		}
		

		i += 1;
	}
}


//Show Castling for the king.
void LegalMove::Castling(ChessPiece board[8][8], int x, int y) {
	bool legal = true;


	if (!board[x][y].hasMoved) {

		if (!board[x + 3][y].hasMoved) {
			for (int j = 1; j <= 2; j++) {

				if (board[x][y].getSide() == 0) {
					if (board[x][y].dangerWhite || board[x + j][y].onBoard || board[x + j][y].dangerWhite) {
						legal = false;
						break;
					}
				}
				else {
					if (board[x][y].dangerBlack || board[x + j][y].onBoard || board[x + j][y].dangerBlack) {
						legal = false;
						break;
					}
				}
			}

			if (legal) {
				board[x][y].kingSpecial = true;

				board[x + 2][y].Selected = true;
			}
		}
		
		legal = true;

		if (!board[x -4][y].hasMoved) {
			for (int j = 1; j <= 2; j++) {

				if (board[x][y].getSide() == 0) {
					if (board[x][y].dangerWhite || board[x - j][y].onBoard || board[x - j][y].dangerWhite) {
						legal = false;
						break;
					}
				}
				else {
					if (board[x][y].dangerBlack || board[x - j][y].onBoard || board[x - j][y].dangerBlack) {
						legal = false;
						break;
					}
				}
			}

			if (legal) {
				board[x][y].kingSpecial = true;

				board[x - 2][y].Selected = true;
			}
		}

		
	}
}

void LegalMove::action(ChessPiece board[8][8], int ix, int iy, int x, int y, bool danger, int side) {
	if (danger) {
		
		if (side == 0) {
			board[x][y].dangerBlack = true;
		}
		else {
			board[x][y].dangerWhite = true;
		}
		
	}else {
		board[x][y].Selected = true;
	}
}

void LegalMove::setDangers(ChessPiece board[8][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[j][i].onBoard) {
				Hightlight(board, j, i, true);
			}
		}
	}
}

void LegalMove::resetHighlight(ChessPiece board[8][8]) {
	for (int i = 0; i < 8; i ++) {
		for (int j = 0; j < 8; j++) {
			board[j][i].Selected = false;
		}
	}
}

void LegalMove::moveWhiteKing(int x, int y) {
	this->whiteKingX = x;
	this->whiteKingY = y;
}
void LegalMove::moveBlackKing(int x, int y) {
	this->blackKingX = x;
	this->blackKingY = y;
}