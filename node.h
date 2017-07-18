#ifndef NODE_H
#define NODE_H

#include "move.h"

class Node{
	public:	
		Node(Move move) {
			this->nextMove = move;
		}
		Node() {}

		void setMove(Move move) {
			this->nextMove = move;
		}

		Move getMove() {
			return this->nextMove;
		}

		void setNext(Node* next) {
			this->next = next;
		}

		Node* getNext() {
			return this->next;
		}


	private:
		Move nextMove;

		Node* next;
};

#endif