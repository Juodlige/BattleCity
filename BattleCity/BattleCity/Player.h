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
		case 'd': nextAction = 1;
		case 's': nextAction = 2;
		case 'a': nextAction = 3;
			//move();
		case 'k': nextAction = 4;
			//shoot();
		default:
			break;
		}
	}

	int doThings() {
		return nextAction;
	}

	void spawn() {
		pos = { 48, 16 };
	}

private:

	int lives;
};