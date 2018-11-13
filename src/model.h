#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader_s.h"
#include "mesh.h"

using namespace std;
using namespace glm;

class Model {
public:
    Model(char const *path);
    void Draw(Shader shader);
    Mesh getMesh(unsigned int &index);
    unsigned int meshSize();
private:
    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type, string typeName);
};
