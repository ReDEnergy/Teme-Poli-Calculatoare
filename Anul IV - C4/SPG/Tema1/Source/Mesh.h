#ifndef MESH_H
#define MESH_H

#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Globals.h"

using namespace std;

#define INVALID_MATERIAL	0xFFFFFFFF
#define INDEX_BUFFER		0
#define POS_VB				1
#define NORMAL_VB			2
#define TEXCOORD_VB			3


struct MaterialBlock {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emissive;
	float shininess;
};

class Material : public MaterialBlock {

public:
	Material() {};
	~Material() {};
	void createUBO();
	void linkShader(Shader* const S);

public:

	Shader *shader;
	GLuint material_ubo;
	Texture* texture;

};

struct MeshEntry {
	MeshEntry()
	{
		NumIndices = 0;
		BaseVertex = 0;
		BaseIndex = 0;
		MaterialIndex = INVALID_MATERIAL;
	}
	unsigned int NumIndices;
	unsigned int BaseVertex;
	unsigned int BaseIndex;
	unsigned int MaterialIndex;
};

class Mesh {

public:
	Mesh();

	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void Render();

private:
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(const aiMesh* paiMesh,
				vector<glm::vec3>& Positions,
				vector<glm::vec3>& Normals,
				vector<glm::vec2>& TexCoords,
				vector<unsigned int>& Indices);

	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	GLuint m_VAO;
	GLuint m_Buffers[4];

	vector<MeshEntry> m_Entries;
	vector<Material*> m_Materials;

public:
	Shader *shader;
};

void color_to_vec4(const aiColor4D &color, glm::vec4 &dest);

#endif