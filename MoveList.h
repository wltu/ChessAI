#ifndef MOVELIST_H
#define MOVELIST_H

#include <iostream>
using namespace std;

#include "node.h"

class MoveList {
	public:
		MoveList() {
			this->head = NULL;
			count = 0;
		}

		int size() {
			return count;
		}

		void add(Move move) {
			Node* temp = new Node(move);

			temp->setNext(head);
			head = temp;

			this->count++;
		}

		void clear() {
			Node* temp = head;

			while (head != NULL) {
				head = head->getNext();
				
				delete temp;

				temp = head;
			}

			this->count = 0;
		}

		Node* getHead() {
			return this->head;
		}

		Move returnMove(int num) {
			Node* temp = head;

			for (int i = 0; i < num - 1; i++) {
				temp = temp->getNext();
			}

			return temp->getMove();
		}

	private:
		Node* head;
		int count;
};

#endif