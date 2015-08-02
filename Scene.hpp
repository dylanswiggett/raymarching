#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <vector>
#include "GL/glew.h"
#include "GL/gl.h"
#include "glm/glm.hpp"

#define VERT_SHADER "shader.vert"
#define FRAG_SHADER "shader.frag"

#define MAX_SPHERES 256

using namespace std;

typedef struct Sphere {
    float x, y, z, rad;
    float r, g, b;
    float light_emit;
} Sphere;

class Scene {
public:
    Scene(int w, int h) : w_(w), h_(h) {
	spheres_ = new vector<Sphere>();
    };

    virtual ~Scene();
    virtual void setCamera(glm::vec3 pos, glm::vec3 dir);
    virtual void setupGL();
    virtual void updateGL();
    virtual void drawGL();
private:
    int w_, h_, t_;
    vector<Sphere> *spheres_;
    GLuint render_buffer_, vertex_buffer_;
    GLuint shader_;
    GLuint vao_;
};

#endif
