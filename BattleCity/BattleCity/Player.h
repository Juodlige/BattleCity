#pragma once
#include "Tank.h"
using namespace std;

class Player : public Tank {
	

public:
	Player(int newID) : Tank(newID){
		health = 1;
		lives = 3;
		direction = 0;
	}

	void readInput(char inp) {

		switch (inp) {
		case 'w': nextAction = 0;
			break;
		case 'd': nextAction = 1;
			break;
		case 's': nextAction = 2;
			break;
		case 'a': nextAction = 3;
			break;
			//move();
		case 'k': nextAction = 4;
			break;
			//shoot();
		default:
			break;
		}
	}

	int doAction() {
		return nextAction;
	}

	void spawn() {
		pos = { 48, 16 };
	}

	void newDirection(int newDir) { direction = newDir; }

private:

	int lives;
};