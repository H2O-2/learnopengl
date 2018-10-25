#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader_s.h"
#include "mesh.h"

using namespace std;
using namespace glm;

class Model {
public:
    Model(char *path);
    void Draw(Shader shader);
private:
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    void processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTexture(aiMaterial *mat, aiTextureType type, string typeName);
};
