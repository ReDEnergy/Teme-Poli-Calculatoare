#pragma once

#include <cstdio>
#include <cstdlib>
#include <time.h>

#include "Globals.h"
#include "Object3D.h"
#include "Player.h"
#include "Shader.h"

using namespace std;

enum CELLTYPE {
	NONE,
	RIGID_WALL,
	WALL,
	BOMB_SET,
	BOMB_GIFT,
	BOMB_WALL,
	EXPLOSION,
	PORTAL,
	PORTAL_WALL,
};

class BombInfo {
public:
	BombInfo();
	~BombInfo();
public:
	float update_time;
	CELLTYPE state;
};

class Map {

	public:
		Map(int width, int length);
		~Map();

		void Map::init(Shader *shader);
		void genBombermanMap();
		void reset();
		void print();
		void render();
		void update(float time, float delta_time);
		void leaveLevel();

		void placeBomb(glm::vec3 position);
		bool moveHero(glm::vec3 direction, float delta_time);
		glm::vec3 getRandomSpawnCellPosition();

		void explodeBomb(int posX, int posY);
		bool destroyCell(int posX, int posY);
		glm::vec3 getMoveDirection(int posX, int posY);


	public:
		bool win;
		float win_time;
		int width;
		int length;
		int nr_bombs;
		int explode_radius;
		float explosion_delay;
		float explosion_duration;
		float explosion_time;
		float wall_size;
		unsigned char **map;
		float time_sice_start;
		Object3D *obj_rigid_wall;
		Object3D *obj_light_wall;
		Object3D *obj_bomb;
		Object3D *obj_explosion;
		Object3D *obj_portal;
		Object3D *obj_gift;
		Player *hero;
		BombInfo **bomb_map;
		vector<Player*> enemies;
};