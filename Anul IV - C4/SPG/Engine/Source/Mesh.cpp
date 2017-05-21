#include "Mesh.h"

Mesh::Mesh()
{
	m_VAO = 0;
	ZERO_MEM(m_Buffers);
}


Mesh::~Mesh()
{
	Clear();
}


void Mesh::Clear()
{

	for (unsigned int i = 0 ; i < m_Materials.size() ; i++) {
		SAFE_FREE(m_Materials[i]);
	}

	if (m_Buffers[0] != 0) {
		glDeleteBuffers(SIZEOF_ARRAY(m_Buffers), m_Buffers);
	}
	   
	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}


bool Mesh::LoadMesh(const string& Filename) {

	// Release the previously loaded mesh (if it exists)
	Clear();
	string Path = "Models\\" + Filename;
 
	// Create the VAO
	glGenVertexArrays(1, &m_VAO);   
	glBindVertexArray(m_VAO);
	
	// Create the buffers for the vertices attributes
	glGenBuffers(SIZEOF_ARRAY(m_Buffers), m_Buffers);

	bool Ret = false;
	Assimp::Importer Importer;

	const aiScene* pScene = Importer.ReadFile(Path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	
	if (pScene) {
		Ret = InitFromScene(pScene, Path);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Path.c_str(), Importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);	

	return Ret;
}

bool Mesh::InitFromScene(const aiScene* pScene, const string& Filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Materials.resize(pScene->mNumMaterials);

	vector<glm::vec3> Positions;
	vector<glm::vec3> Normals;
	vector<glm::vec2> TexCoords;
	vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;
	
	// Count the number of vertices and indices
	for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;      
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;
		
		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices  += m_Entries[i].NumIndices;
	}
	
	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Indices.reserve(NumIndices);

	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(paiMesh, Positions, Normals, TexCoords, Indices);
	}

	if (!InitMaterials(pScene, Filename))
		return false;

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);    

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return (CheckOpenGLError() == GL_NO_ERROR);
}

void Mesh::InitMesh(const aiMesh* paiMesh,
					vector<glm::vec3>& Positions,
					vector<glm::vec3>& Normals,
					vector<glm::vec2>& TexCoords,
					vector<unsigned int>& Indices)
{    
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
		const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	// Populate the index buffer
	for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

bool Mesh::InitMaterials(const aiScene* pScene, const string& Filename)
{
	// Extract the directory part from the file name
	string::size_type SlashIndex = Filename.find_last_of("/");
	string Dir = "Textures\\";

	bool ret = true;

	// Initialize the materials
	for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Materials[i] = new Material();
		m_Materials[i]->linkShader(shader);

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				string FullPath = Dir + Path.data;
				m_Materials[i]->texture = new Texture();

				if (!m_Materials[i]->texture->load2D(FullPath.c_str())) {
					delete m_Materials[i]->texture;
					m_Materials[i]->texture = NULL;
					ret = false;
				}
			}

			aiColor4D color;

			if(aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS)
				color_to_vec4(color, m_Materials[i]->ambient);

			if(aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS)
				color_to_vec4(color, m_Materials[i]->diffuse);

			if(aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS)
				color_to_vec4(color, m_Materials[i]->specular);

			if(aiGetMaterialColor(pMaterial, AI_MATKEY_COLOR_EMISSIVE, &color) == AI_SUCCESS)
				color_to_vec4(color, m_Materials[i]->emissive);

			unsigned int max;
			aiGetMaterialFloatArray(pMaterial, AI_MATKEY_SHININESS, &m_Materials[i]->shininess, &max);

			m_Materials[i]->createUBO();
		}
	}

	CheckOpenGLError();

	return ret;
}


void Mesh::Render()
{
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {

		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
		assert(MaterialIndex < m_Materials.size());
		
		if (m_Materials[MaterialIndex]->texture) {
			m_Materials[MaterialIndex]->texture->bind(GL_TEXTURE0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_Materials[MaterialIndex]->material_ubo);
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
								 m_Entries[i].NumIndices, 
								 GL_UNSIGNED_INT, 
								 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex), 
								 m_Entries[i].BaseVertex);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	glBindVertexArray(0);
}


void color_to_vec4(const aiColor4D &color, glm::vec4 &dest)
{
	dest.r = color.r;
	dest.g = color.g;
	dest.b = color.b;
	dest.a = color.a;
}

/**
 * Material Class
 */

void Material::createUBO() {
	glGenBuffers(1, &material_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, material_ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialBlock), static_cast<MaterialBlock*>(this), GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if (shader->loc_material != INVALID_LOC)
		glUniformBlockBinding(shader->program, shader->loc_material, 0);
};

void Material::bindUBO(GLuint location) {
	if (shader->loc_material != INVALID_LOC)
		glUniformBlockBinding(shader->program, shader->loc_material, location);
}

void Material::linkShader(Shader* const S) {
	shader = S;
}
