#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "CORE_RL_Magick++_.lib")
#pragma comment(lib, "glew32.lib") 
#pragma comment(lib, "assimp.lib") 


#include <cstdio>
#include <cstdlib>
#include <Magick++.h>

#include "Engine.h"
#include "BomberMan.h"

using namespace std;

#define IMAGE_MAGICK_PATH "D:\\Programs\\ImageMagick-6.8.7-Q16\\"

int main() {

	// Magick::InitializeMagick(IMAGE_MAGICK_PATH);
	srand(time(NULL));

	Engine::init();
	BomberMan *BMan = new BomberMan();
	Engine::setGameObject(BMan);
	Engine::run();

	return 0;
}
