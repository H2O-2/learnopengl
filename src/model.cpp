#include "model.h"

Model::Model(char *path) {

}

void Model::Draw(Shader shader) {
    for (int i = 0; i < meshes.size(); ++i) {
        meshes[i].Draw(shader);
    }
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
}
