#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <assmip/Importer.hpp>

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
    void processNode();
};
