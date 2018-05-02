//Willy Tu
//Start Date: 2/10/2017

#include "board.h"

void display(void);
void adjustSize(int w, int h);
void mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

Board board = Board();

int main(int argc, char **argv) {
	int size = board.getSize();

	cout << "Welcome to Chess!" << endl;
	cout << "Press \"R\" to restart the game." << endl;
	cout << "Press \"Esc\" to exit the game." << endl;

	// init GLUT and create main window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(size, size);
	glutCreateWindow("Chess");

	// Callbacks for main window
	glutDisplayFunc(display);
	glutReshapeFunc(adjustSize);

	// Mouse Input
	glutMouseFunc(mouse);

	// Keyboard Input
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 1;
}

void display(void) {
	board.draw();
}

void adjustSize(int w, int h) {
	board.fixedSize(w, h);
}


void mouse(int button, int state, int x, int y) {
	board.mouseInput(button, state, x, y);
}

void keyboard(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);
	}
	else if (key == 'r') {
		board.reset();
	}
}
