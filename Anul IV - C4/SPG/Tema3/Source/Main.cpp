#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "assimp.lib")

#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "Engine.h"
#include "Game.h"

using namespace std;

int main() {

	srand((unsigned int)time(NULL));

	Engine::init();
	Game *game = new Game();
	Engine::setWorldInstance(game);
	Engine::run();

	return 0;
}
