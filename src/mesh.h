#pragma once

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader_s.h"

using namespace std;
using namespace glm;

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 Tangent;
    vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> texture;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> texture);
    void Draw(Shader shader);
private:
    unsigned VAO, VBO, EBO;
    const string DIFFUSE = "texture_diffuse";
    const string SPECULAR = "texture_specular";
    const string NORMAL = "texture_normal";
    const string HEIGHT = "texture_height";

    void setupMesh();
};
