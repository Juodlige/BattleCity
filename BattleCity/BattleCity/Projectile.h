using namespace std;

class Projectile {
private:
	int parentID;
	int direction;
	pair <int, int> pos;

public:
	Projectile(int newPID, pair <int, int> newPos, int newDir) {
		parentID = newPID;
		pos = newPos;
		direction = newDir;
	}

	void move() {
		switch (direction)
		{
		case 0:
			pos.first -= 2;
			break;
		case 1:
			pos.second += 2;
			break;
		case 2:
			pos.first += 2;
			break;
		case 3:
			pos.second -= 2;
			break;
		default:
			break;
		}
	}

	pair <int, int> getPos() { return pos; }
	int getPID() { return parentID; }
};