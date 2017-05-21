#include "Map.h"

#include <fstream>


glm::vec3 direction[4] = { 
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, -1.0f)
};


BombInfo::BombInfo() {
	state = NONE;
}

BombInfo::~BombInfo() {

}

/*
 * Map Class
 */

Map::Map(int width, int length) {

	this->width = width;
	this->length = length;
	wall_size = 2.0f;
	time_sice_start = 0;
	nr_bombs = 3;
	explode_radius = 2;
	explosion_delay = 3.0f;
	explosion_duration = 0.4f;
	win = false;
}

void Map::init(Shader *shader) {
	
	hero = new Player(shader, "Bman.obj");
	hero->scale = glm::vec3(0.15f, 0.15f, 0.15f);
	hero->position = glm::vec3(2.0f, 0.0f, 2.0f);

	Player *P;
	for (int i=0; i<2; i++) {
		P = new Player(shader, "Bman.obj");
		P->scale = glm::vec3(0.15f, 0.15f, 0.15f);
		P->position = getRandomSpawnCellPosition();
		P->cellX = int(P->position.x / 2);
		P->cellY = int(P->position.z / 2);
		enemies.push_back(P);
	}

	obj_light_wall = new Object3D(shader, "brick_wall.obj");
	obj_light_wall->scale = glm::vec3(0.1f, 0.1f, 0.1f);

	obj_rigid_wall = new Object3D(shader, "concrete_wall.obj");
	obj_rigid_wall->scale = glm::vec3(0.1f, 0.1f, 0.1f);
	obj_rigid_wall->position = glm::vec3(2.0f, 0.0f, 0.0f);

	obj_bomb = new Object3D(shader, "Bomb.obj");
	obj_bomb->scale = glm::vec3(0.1f, 0.1f, 0.1f);

	obj_explosion = new Object3D(shader, "explosion.obj");
	obj_explosion->scale = glm::vec3(0.1f, 0.1f, 0.1f);

	obj_portal = new Object3D(shader, "portalbutton.obj");
	obj_portal->scale = glm::vec3(0.02f, 0.02f, 0.02f);

	obj_gift = new Object3D(shader, "dinamite.obj");
	obj_gift->scale = glm::vec3(0.1f, 0.1f, 0.1f);

}

void Map::genBombermanMap() {

	map = new unsigned char*[width];
	bomb_map = new BombInfo*[width];

	for (int i=0; i<width; i++) {
		map[i] = new unsigned char[length];
		bomb_map[i] = new BombInfo[length];
	}

	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < length - 1; j++) {

			map[i][j] = NONE;

			if (rand() % 2 == 1)
				if (rand() % 2 == 0)
				map[i][j] = WALL;

			if (rand() % 2 == 0)
				if (rand() % 2 == 1)
					map[i][j] = WALL;

			if ((i % 2 == 0 && j % 2 == 0))
				map[i][j] = RIGID_WALL;
		}
	}

	for (int i = 0; i < width; i++) {
		map[i][0] = RIGID_WALL;
		map[i][width-1] = RIGID_WALL;
	}

	for (int i = 0; i < length; i++) {
		map[0][i] = RIGID_WALL;
		map[length-1][i] = RIGID_WALL;
	}

	map[1][1] = NONE;
	map[1][2] = NONE;
	map[2][1] = NONE;

	while (1) {
		int x = rand() % width/2 + width/2 - 1;
		int y = rand() % length/2 + length/2 - 1;
		if (map[x][y] == WALL || map[x][y] == NONE) {
			map[x][y] = PORTAL_WALL;
			break;
		}
	}

	while (1) {
		int x = rand() % width/2 + width/2 - 1;
		int y = rand() % length/2 + length/2 - 1;
		if (map[x][y] == WALL || map[x][y] == NONE) {
			map[x][y] = BOMB_WALL;
			break;
		}
	}
}


void Map::reset() {
	genBombermanMap();
	hero->position = glm::vec3(2.0f, 0.0f, 2.0f);
	for (int i=0; i<enemies.size(); i++) {
		enemies[i]->position = getRandomSpawnCellPosition();
		enemies[i]->cellX = int(enemies[i]->position.x / 2);
		enemies[i]->cellY = int(enemies[i]->position.z / 2);
		enemies[i]->alive = true;
		enemies[i]->state = false;
	}
	nr_bombs = 3;
	win = false;
}

void Map::leaveLevel() {
	if ((time_sice_start - win_time) < 1.0f) {
		hero->angle +=20;
		hero->position += glm::vec3(0.0f, -0.01f, 0.0f);
	}
	else
		reset();
}

void Map::update(float time, float delta_time) {
	time_sice_start = time;

	if (win == true) {
		leaveLevel();
		return;
	}

	for (int i=1; i<width-1; i++) {
		for (int j=1; j<length-1; j++) {
			switch (bomb_map[i][j].state) {

				case BOMB_SET:
					if ((time - bomb_map[i][j].update_time) > explosion_delay) {
						explosion_time = bomb_map[i][j].update_time + explosion_delay;
						explodeBomb(i, j);
					}
					break;

				case EXPLOSION:
					if ((time - bomb_map[i][j].update_time) > explosion_duration) {
						bomb_map[i][j].state = NONE;
						map[i][j] = NONE;
					}
					break;

				default:
					break;
			}
		}
	}

	for (int i=0; i<enemies.size(); i++) {
		if (enemies[i]->alive == false)
			continue;

		if (glm::distance(hero->position, enemies[i]->position) < 1) {
			cout << "IDIOT YOU DIED" << endl;
			reset();
		}
			
		if (map[enemies[i]->cellX][enemies[i]->cellY] == EXPLOSION) {
			enemies[i]->alive = false;
			cout << "Enemy killed!" << endl;
			continue;
		}

		if (enemies[i]->state) {
			enemies[i]->moveToDestination(delta_time);
		}
		else {
			enemies[i]->move_direction = getMoveDirection(enemies[i]->cellX, enemies[i]->cellY);
			if (enemies[i]->move_direction == glm::vec3(0))
				break;
			enemies[i]->start_position = enemies[i]->position;
			enemies[i]->state = true;
		}
	}
}


bool Map::moveHero(glm::vec3 direction, float delta_time) {
	bool ret = true;
	glm::vec3 pos = hero->getExpectedPos(direction, delta_time);

	int cellX = int((pos.x + 1.5) / wall_size);
	int cellY = int((pos.z + 1.5) / wall_size);

	if (map[cellX][cellY] == PORTAL &&
		abs(pos.x - (cellX * 2)) < 0.2f && 
		abs(pos.z - (cellY * 2)) < 0.2f) {
		cout << "YOU WIN" << endl;
		win = true;
		win_time = time_sice_start;
		return false;
	}

	if (map[cellX][cellY] == BOMB_GIFT &&
		abs(pos.x - (cellX * 2)) < 0.2f && 
		abs(pos.z - (cellY * 2)) < 0.2f) {
		cout << "YOU RECEIVED ONE ADDITIONAL BOMB" << endl;
		nr_bombs++;
		map[cellX][cellY] = NONE;
	}

	if (map[cellX][cellY] == EXPLOSION) return true;
	if (map[cellX][cellY] != NONE &&
		map[cellX][cellY] != BOMB_SET &&
		map[cellX][cellY] != PORTAL &&
		map[cellX][cellY] != BOMB_GIFT)
		return true;

	cellX = int((pos.x + 0.5) / wall_size);
	cellY = int((pos.z + 0.5) / wall_size);

	if (map[cellX][cellY] == NONE)		ret = false;
	if (map[cellX][cellY] == BOMB_SET)	ret = false;
	if (map[cellX][cellY] == PORTAL)	ret = false;
	if (map[cellX][cellY] == BOMB_GIFT)	ret = false;

	if (ret == false) {
		hero->move(direction, delta_time);
		hero->cellX = cellX;
		hero->cellY = cellY;
	}

	return ret;
}

void Map::placeBomb(glm::vec3 position) {

	if (nr_bombs == 0)
		return;

	int cellX = int((position.x + 1) / wall_size);
	int cellY = int((position.z + 1) / wall_size);

	if (map[cellX][cellY] == BOMB_SET ||
		map[cellX][cellY] == RIGID_WALL ||
		map[cellX][cellY] == WALL)
		return;

	map[cellX][cellY] = BOMB_SET;

	bomb_map[cellX][cellY].update_time = time_sice_start;
	bomb_map[cellX][cellY].state = BOMB_SET;

	nr_bombs--;
}

void Map::explodeBomb(int posX, int posY) {
	 
	int x = posX;
	int y = posY;

	map[posX][posY] = EXPLOSION;
	bomb_map[posX][posY].state = EXPLOSION;
	bomb_map[posX][posY].update_time = explosion_time;

	while(x-- > 1) {
		if ((posX - x) > explode_radius)	break;
		if (destroyCell(x, posY) == true)	break;
	}
	x = posX;
	while(x++ < width) {
		if ((x - posX) > explode_radius)	break;
		if (destroyCell(x, posY) == true)	break;
	}
	y = posY;
	while(y-- > 1) {
		if ((posY - y) > explode_radius)	break;
		if (destroyCell(posX, y) == true)	break;
	}
	y = posY;
	while(y++ < length) {
		if ((y - posY) > explode_radius)	break;
		if (destroyCell(posX, y) == true)	break;
	}

	nr_bombs++;
};

bool Map::destroyCell(int posX, int posY) {

	if (hero->cellX == posX && hero->cellY == posY) {
		cout << "YOU JUST DIED! MORE LUCK NEXT TIME" << endl;
		reset();
	}

	if (map[posX][posY] == NONE) {
		map[posX][posY] = EXPLOSION;
		bomb_map[posX][posY].state = EXPLOSION;
		bomb_map[posX][posY].update_time = explosion_time;
		return false;
	}

	if (map[posX][posY] == RIGID_WALL)
		return true;

	if (map[posX][posY] == EXPLOSION)
		return true;

	if (map[posX][posY] == BOMB_SET) {
		explodeBomb(posX, posY);
		return true;
	}

	if (map[posX][posY] == WALL) {
		map[posX][posY] = NONE;
		return true;
	}

	if (map[posX][posY] == BOMB_WALL) {
		map[posX][posY] = BOMB_GIFT;
		return true;
	}

	if (map[posX][posY] == PORTAL_WALL) {
		map[posX][posY] = PORTAL;
		return true;
	}

	return false;
 }

glm::vec3 Map::getRandomSpawnCellPosition() {
	glm::vec3 V;
	int cellX, cellY, x = 0;

	while (1) {
		cellX = (rand() % (width/2)) + width/2;
		cellY = (rand() % (length/2)) + length/2;
		
		if (rand() % 2)
			cellX -= width/2 - 1;
		else
			cellY -= length/2 - 1;

		if (map[cellX][cellY] == NONE) {
			V.x = cellX * wall_size;
			V.z = cellY * wall_size;
			break;
		}
	}

	V.y = 0;
	log_vec3(V, "random EMPTY cell");
	return V;
}

glm::vec3 Map::getMoveDirection(int posX, int posY) {
	int rand_dir, newX, newY;

	rand_dir = rand() % 4;
	newX = posX + (int)direction[rand_dir].x;
	newY = posY + (int)direction[rand_dir].z;

	if (newX < 0 || newX == width || newY < 0 || newY == length)
		return glm::vec3(0);

	if (map[newX][newY] == NONE)
		return direction[rand_dir];

	return glm::vec3(0);
}

void Map::render() {
	float posX = 0;
	float posZ = 0;
	glm::vec3 position;

	for (int i = 0; i < width; i++) {
		posZ = 0;
		for (int j = 0; j < length; j++) {
			position = glm::vec3(posX, 0, posZ);
			if (map[i][j] == RIGID_WALL) {
				obj_rigid_wall->position = position;
				obj_rigid_wall->render();
			}
			if (map[i][j] == WALL || map[i][j] == PORTAL_WALL || map[i][j] == BOMB_WALL) {
				obj_light_wall->position = position;
				obj_light_wall->render();
			}

			if (map[i][j] == BOMB_SET) {
				obj_bomb->position = position;
				obj_bomb->scale = glm::vec3( 0.5f - abs(0.1f * cos(time_sice_start * M_PI)));
				obj_bomb->render();
			}

			if (map[i][j] == EXPLOSION) {
				obj_explosion->position = position;
				obj_explosion->render();
			}

			if (map[i][j] == PORTAL) {
				obj_portal->position = position;
				obj_portal->render();
			}

			if (map[i][j] == BOMB_GIFT) {
				obj_gift->position = position;
				obj_gift->render();
			}


			posZ += wall_size;
		}
		posX += wall_size;
	}

	for (int i=0; i<enemies.size(); i++)
		if (enemies[i]->alive)
			enemies[i]->render();

	hero->render();
}

/*
 * Printeaza la output modelul labirintului
 */
void Map::print() {
	for (int i = 0; i < width; i++) {
		cout << endl;
		for (int j = 0; j < length; j++) {
			cout << map[i][j];
		}
	}
	cout << endl << endl;
}