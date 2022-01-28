#pragma once
using namespace std;



class Tank {
protected:
	int ID;
	int health;
	int shotCount, shotCooldown;
	int direction;
	int nextAction; 
	pair <int, int> pos; //X,Y

	void die() {
		pos = { 99,99 };
	}

public:

	void move(pair <int, int> newpos) {
		pos.first += newpos.first;
		pos.second += newpos.second;
	}

	Tank(int newID) {
		ID = newID;
		pos.first = 99; //spawns out of bounds;
		pos.second = 99;
		nextAction = 5;
		direction = 2;
	}

	int doThings(int x) {	
		nextAction = decideAction(x);
		//if (nextAction >= 0 && nextAction <= 3) {
		//	direction = nextAction;
		//}
		return nextAction;
	}					

	int getID() { 
		return ID; 
	}

	int getDirection() { 
		return direction; 
	}

	pair <int, int> getPos() { 
		return pos; 
	}

	void takeDamage(int damageTaken){
		health -= damageTaken;
		if (health <= 0) {
			die();
		}
	}

	void spawn(int x) {
		direction = 2;
		pos = { 0, x };
	}

	void newDirection(int x) {
		direction = x;
	}

private:
	
	int decideAction(int x) {
		vector<int> actionPool = {0, 1, 2, 3, 4, 4, 5, 5, 5};

		if (pos.first == 99) {
			return 6;
		}
		if (pos.first < 24) {		 //top part of map
			actionPool.push_back(2); //adds down
			actionPool.push_back(4); //forward
		}
		else {
			actionPool.push_back(5); //adds 2x shoot
			actionPool.push_back(5);
		}
		if (pos.second < 24) {		 //left side
			actionPool.push_back(1); //adds right
		}
		else {
			actionPool.push_back(3); //adds left
		}
		return actionPool[x];
	}
};
