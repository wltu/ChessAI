#ifndef MOVELIST_H
#define MOVELIST_H

#include <iostream>
using namespace std;

#include "node.h"

class MoveList {
	public:
		MoveList() {
			head = NULL;
		}

		void add(Move move, int points) {
			Node* temp = new Node(move, points);

			temp->setNext(head);
			head = temp;
		}

		int peek(int num) {
			Node* temp = head;

			for (int i = 0; i < num - 1; i++) {
				temp = temp->getNext();
			}

			return temp->getPoints();
		}

		Move returnMove(int num) {
			Node* temp = head;

			for (int i = 0; i < num - 1; i++) {
				temp = temp->getNext();
			}

			return temp->getMove();
		}

		void remove(int num) {
			Node* temp = head;
			Node* temp2;

			for (int i = 1; i < num - 1; i++) {
				temp = temp->getNext();
			}

			if (num == 1) {
				this->head = temp->getNext();
			}
			else {
				temp2 = temp->getNext()->getNext();

				delete temp->getNext();

				temp->setNext(temp2);
			}
		}

		bool empty() {
			if (this->head == NULL) {
				return true;
			}
			else {
				return false;
			}
		}

		Node* getHead() {
			return this->head;
		}

	private:
		Node* head;
};

#endif