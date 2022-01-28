#include "stdafx.h"
using namespace std;

struct tile {
	int MapTile;
	int FilledTile;
};

struct entityInfo {
	pair<int, int> pos;
	int type;
	int direction;
};

void Display(vector<vector<tile>> Map);
void readLevel(string name, vector<vector<tile>>& Map, vector<int>& TankList);
char Interpreter(int tile);
void mapUpdate(vector<vector<tile>>& Map, vector<entityInfo> EntityPositions);
void posUpdate(vector<entityInfo>& EntityPositions, Player p1, vector<Tank> Enemy, vector<int>& SpawnQue);
void mapReset(vector<vector<tile>>& Map);
void Spawn(vector<vector<tile>>& Map, vector<int>& SpawnQue, int& spawnSelector, Player& p1, vector<Tank>& Enemy);
bool isTileEmpty(vector<vector<tile>> Map, int toCheck);
bool isInQue(vector<int>& SpawnQue, int position);
void botsCollectActions(vector<int>& actions, vector<Tank>& Enemy);
void tanksDoAction(vector<int>& actions, vector<Tank>& Enemy, Player p1, vector<vector<tile>> Map);
//int displayAllInfo(vector<entityInfo> EntityPositions, Player p1, vector<Tank> Enemy);

int main()
{
	//Initialize
	vector<vector<tile>> Map;
	vector<int> TankList;
	vector<entityInfo> EntityPositions;
	vector<int> SpawnQue = { 0,0,0,0,0 };
	vector<int> actions = { 0,0,0,0,0,0 };
	int spawnSelector = 0;
	string levelName = "level1.txt";	//TODO change into menu + more levels
	Player p1 = Player(0);
	vector<Tank> Enemy;
	for(int i = 0; i<5; i++) Enemy.push_back(Tank(i+1));
	readLevel(levelName, Map, TankList);
	char buttonPress;
	srand(time(0));

	//GameCycle
	while (true) {
		mapReset(Map);
		posUpdate(EntityPositions, p1, Enemy, SpawnQue);
		Spawn(Map, SpawnQue, spawnSelector, p1, Enemy);
		mapUpdate(Map, EntityPositions);
		Display(Map);
		//displayAllInfo(EntityPositions, p1, Enemy);
		buttonPress = _getch();
		p1.readInput(buttonPress);
		actions[0] = p1.doThings();
		botsCollectActions(actions, Enemy);
		tanksDoAction(actions, Enemy, p1, Map);
	}
	//TODO movements

	
    return 0;
}

//displays graphics
void Display(vector<vector<tile>> Map) {

	string a, b;
	a = "------------------------------------------------------";
	cout << a << endl;
	for (int i = 0; i < 52; i++) {
		cout << "|";
		for (int j = 0; j < 52; j++) {
			
			cout << Interpreter(Map[i][j].FilledTile);
		}
		cout << "|";
		cout << endl;
	}
	cout << a << endl;

}

//interprets numbers as game "graphics"
char Interpreter(int tile) {
	switch (tile) {
	case 0:
		return 32; //' ' empty
	case 1:
		return 79; //'O' destructible
	case 2:
		return 35; //'#' indestructible
	case 3:
		return 64; //'@' hides
	case 4:
		return 126;//'~' immpassable
	case 5:
		return 36; //'&' base
	case 6:
		return 79; // 'O' Player tank body
	case 7:
		return 61; // '=' Player tank gun
	case 8:
		return 81; // 'Q' Enemy tank body
	case 9:
		return 45; // '-' Enemy tank gun
	default:
		return 32;
	}
}

//intial file read
void readLevel(string name, vector<vector<tile>>& Map, vector<int>& TankList) {
	int x, type;
	tile a;
	ifstream fr;
	fr.open(name);
	for (int i = 0; i < 4; i++) {
		
		fr >> type >> x;

		for(int j=0; j<x; j++) TankList.push_back(type);
	}
	for (int i = 0; i < 52; i++) {
		
		Map.push_back(vector<tile>());
		for (int j = 0; j < 52; j++) {
			fr >> a.MapTile;
			a.FilledTile = a.MapTile;
			Map[i].push_back(a);
		}
		
	}
	fr.close();
}

//updates map with current positions
void mapUpdate(vector<vector<tile>>& Map, vector<entityInfo> EntityPositions) {
	
	int tankbody;
	int tankgun;
	int howmanytanks = EntityPositions.size();
	
	for (int i = 0; i < howmanytanks; i++) {

		if (EntityPositions[i].pos.first > 52) continue; // if tanks is inbounds, it gets displayed
		
		switch (EntityPositions[i].type) {
		case 1:
			tankbody = 6;
			tankgun = 7;
			break;
		case 2:
			tankbody = 8;
			tankgun = 9;
			break;
		default:
			tankbody = 8;
			tankgun = 9;
			break;
		}
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				Map[EntityPositions[i].pos.first + k][EntityPositions[i].pos.second + j].FilledTile = tankbody;
			}
		}
		for (int j = 1; j <= 2; j++) {
			for (int k = 1; k <= 2; k++) {
				Map[EntityPositions[i].pos.first + k][EntityPositions[i].pos.second + j].FilledTile = tankgun;
			}
		}

		switch (EntityPositions[i].direction) {
		case 0: //1,2 1,3
			Map[EntityPositions[i].pos.first + 0][EntityPositions[i].pos.second + 1].FilledTile = tankgun;  //6776
			Map[EntityPositions[i].pos.first + 0][EntityPositions[i].pos.second + 2].FilledTile = tankgun;  //6776
			break;																							//6776
																											//6666
		case 1: //2,4 3,4
			Map[EntityPositions[i].pos.first + 1][EntityPositions[i].pos.second + 3].FilledTile = tankgun;  //6666
			Map[EntityPositions[i].pos.first + 2][EntityPositions[i].pos.second + 3].FilledTile = tankgun;  //6777
			break;																							//6777
																											//6666
		case 2: //4,2 4,3
			Map[EntityPositions[i].pos.first + 3][EntityPositions[i].pos.second + 1].FilledTile = tankgun;  //6666
			Map[EntityPositions[i].pos.first + 3][EntityPositions[i].pos.second + 2].FilledTile = tankgun;  //6776
			break;																							//6776
																											//6776
		case 3: //2,1 3,1
			Map[EntityPositions[i].pos.first + 1][EntityPositions[i].pos.second + 0].FilledTile = tankgun;  //6666
			Map[EntityPositions[i].pos.first + 2][EntityPositions[i].pos.second + 0].FilledTile = tankgun;  //7776
			break;																							//7776
																											//6666
		default:
			break;
		}
	}
	
	
}//for(int x=EntityPositions[i].pos.first(); )

//reads positions of tanks
void posUpdate(vector<entityInfo>& EntityPositions, Player p1, vector<Tank> Enemy, vector<int>& SpawnQue) {
	entityInfo x;
	if (EntityPositions.size() == 0) {
		x.pos = p1.getPos();
		x.type = 1;
		x.direction = p1.getDirection();
		EntityPositions.push_back(x);
		for (int i = 0; i < 5; i++) {

			x.pos = Enemy[i].getPos();
			x.type = 2;
			x.direction = Enemy[i].getDirection();
			EntityPositions.push_back(x);

		}
	}

	else {
		EntityPositions[0].pos = p1.getPos();
		EntityPositions[0].direction = p1.getDirection();
		for (int i = 0; i < 5; i++) {
			EntityPositions[i + 1].pos = Enemy[i].getPos();
			if (EntityPositions[i + 1].pos.first > 52 && !isInQue(SpawnQue, i)) SpawnQue[i] = 1;
			EntityPositions[i + 1].direction = Enemy[i].getDirection();
		}
	}
}

//resets graphics to overlay tanks into map
void mapReset(vector<vector<tile>>& Map) {
	int x = Map.size();
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < x; j++) {
			Map[i][j].FilledTile = Map[i][j].MapTile;
		}
	}
	
}

//spanws tanks
void Spawn(vector<vector<tile>>& Map, vector<int>& SpawnQue, int& spawnSelector, Player& p1, vector<Tank>& Enemy) {
	pair<int, int> x = p1.getPos();
	if (x.first > 52) p1.spawn();

	int spawnPosSelector = (spawnSelector % 3) * 24;
	int spawnedTankSelector = spawnSelector % 5;
	
	int spawnYpos = spawnPosSelector; // 0, 24, 48
	if (isTileEmpty(Map, spawnYpos)) {
		if (SpawnQue[spawnedTankSelector] == 1) {
			Enemy[spawnedTankSelector].spawn(spawnYpos); // %5 = 0,1,2,3,4
			SpawnQue[spawnedTankSelector] = 0;
		}
	}
	spawnSelector++;
}

bool isTileEmpty(vector<vector<tile>> Map, int toCheck) {
	for (int i = 0; i < 4; i++) {
		for (int j = toCheck; j < toCheck + 4; j++) {
			if (Map[i][j].FilledTile != 0) return false;
		}
	}
	return true;
}

bool isInQue(vector<int>& SpawnQue, int position) {
	if (SpawnQue[position] == 0) return false;
	else return true;
}

bool isWallFront(vector<vector<tile>> Map, pair<int, int> pos, int direction) {
	switch (direction) {
	case 0:
		if (pos.first > 0) {
			for (int i = 0; i < 4; i++) {
				if (Map[pos.first - 1][pos.second + i].FilledTile != 0) return true;
			}
		}
		else return true;
		break;
	case 1:
		if (pos.second < 48) {
			for (int i = 0; i < 4; i++) {
				if (Map[pos.first + i][pos.second + 4].FilledTile != 0) return true;
			}
		}
		else return true;
		break;
	case 2:
		if (pos.first < 48) {
			for (int i = 3; i >= 0; i--) {
				if (Map[pos.first + 4][pos.second + i].FilledTile != 0) return true;
			}
		}
		else return true;
		break;
	case 3:
		if (pos.second < 0) {
			for (int i = 0; i < 4; i++) {
				if (Map[pos.first + i][pos.second - 1].FilledTile != 0) return true;
			}
		}
		else return true;
		break;
	default: return true;
	}
	return true;
}

void botsCollectActions(vector<int>& actions, vector<Tank>& Enemy){
	int x = Enemy.size();
	for (int i = 0; i < x; i++) {
		actions[i+1] = Enemy[i].doThings(rand()%12);
	}
}

void tanksDoAction(vector<int>& actions, vector<Tank>& Enemy, Player p1, vector<vector<tile>> Map) {
	int x = actions.size();
	for (int i = 0; i < x; i++) {
		switch (i) {
		case 0: 
			if (actions[i] == p1.getDirection()) {
				if (!isWallFront(Map, p1.getPos(), p1.getDirection())) {
					switch (p1.getDirection()) {
					case 0:
						p1.move({ -1,0 });
						break;
					case 1:
						p1.move({ 0,1 });
						break;
					case 2:
						p1.move({ 1,0 });
						break;
					case 3:
						p1.move({ 0,-1 });
						break;
					default:
						break;
					}
				}
			}
			else p1.newDirection(actions[i]);
		default: 
			if (actions[i] == 4) {
				if (!isWallFront(Map, p1.getPos(), p1.getDirection())) {
					switch (p1.getDirection()) {
					case 0:
						p1.move({ -1,0 });
						break;
					case 1:
						p1.move({ 0,1 });
						break;
					case 2:
						p1.move({ 1,0 });
						break;
					case 3:
						p1.move({ 0,-1 });
						break;
					default:
						break;
					}
				}
			}
			else if (actions[i] >= 0 && actions[i] <= 3) Enemy[i - 1].newDirection(actions[i]);
		}
	}
	
}

/*int displayAllInfo(vector<entityInfo> EntityPositions, Player p1, vector<Tank> Enemy) {
	int x = EntityPositions.size();
	pair <int, int> a;
	for (int i = 0; i < x; i++){
		a = EntityPositions[i].pos;
		cout << i << " - " << a.first << " " << a.second << endl;
		cout << EntityPositions[i].type << " " << EntityPositions[i].direction << endl;
		switch (i) {
		case 0:
			cout << p1.getPos().first << " " << p1.getPos().second << endl;
			break;
		default:
			cout << Enemy[i-1].getPos().first << " " << Enemy[i - 1].getPos().second << endl;
			break;
		}
		//cout << EntityPositions[i].type << " " << EntityPositions[i].direction << endl;
	}
	return 0;
}*/
