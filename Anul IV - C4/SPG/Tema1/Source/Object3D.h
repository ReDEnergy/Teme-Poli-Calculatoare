#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Globals.h"

using namespace std;

/*
 * Class Object3D
 */

class Object3D {

	public:
		Object3D(Shader *shader_program, const string& mesh_file);
		virtual ~Object3D();

		void update();
		void computeModelMatrix();
		void render();

	private:

		Mesh *mesh;
		Shader *shader;

	public:
		glm::mat4 Model;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotation;
		float angle;				// rotation angle in degree
		float move_speed;
};

#endif