#include "Scene.hpp"
#include "GL/glew.h"
#include "GL/gl.h"
#include "glm/glm.hpp"
#include "shader_loader.hpp"
#include <iostream>
#include <string.h>
#include "math.h"


/*                    ,-------------------------------------,
 *  *------*   ^   ^  ( This polygon fills the whole screen! ) 
 *  |  __/ |   O   O   \ ,----------------------------------/
 *  |_/    |    _L_   _|/
 *  *------*   \___/
 */
float frameShape[18] =
    { -1, -1, 0,
      -1,  1, 0,
       1,  1, 0,
      -1, -1, 0,
       1,  1, 0,
       1, -1, 0 };

void printOGLErr() {
    GLuint err;
    cout << "Printing errors:" << endl;
    while ((err = glGetError()) != GL_NO_ERROR) {
        cerr << "OpenGL error: " << err << endl;
    }
}

Scene::~Scene() {
    glDeleteBuffers(1, &render_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);
    delete spheres_;
}

void Scene::setupGL() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // Get single render polygon set up.
    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameShape), frameShape, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // 1282 ???

    glGenBuffers(1, &render_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, render_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Sphere) * MAX_SPHERES, NULL, GL_DYNAMIC_DRAW);

    shader_ = LoadShaders(VERT_SHADER, FRAG_SHADER);
    glUseProgram(shader_);

    // TODO: Initializing the scene should go here.
    
    /*
    // Test spheres
    Sphere s;
    s.x = 1;
    s.y = 1;
    s.z = 1.5;
    s.r = .8;
    s.g = .5;
    s.b = .5;
    s.rad = .2;
    s.light_emit = 1;
    spheres_->push_back(s);
    */

    updateGL();
}

void Scene::setCamera(glm::vec3 pos, glm::vec3 dir) {
  GLuint campos   = glGetUniformLocation(shader_, "campos");
  GLuint camdir   = glGetUniformLocation(shader_, "camdir");
  GLuint camup    = glGetUniformLocation(shader_, "camup");
  GLuint camright = glGetUniformLocation(shader_, "camright");

  glm::vec3 up(0,1,0);
  glm::vec3 right = glm::normalize(glm::cross(up, dir));
  up = glm::normalize(glm::cross(right, dir));

  glUniform3f(campos, pos.x, pos.y, pos.z);
  glUniform3f(camdir, dir.x, dir.y, dir.z);
  glUniform3f(camup, up.x, up.y, up.z);
  glUniform3f(camright, right.x, right.y, right.z);
}

void Scene::updateGL() {
    glBindBuffer(GL_UNIFORM_BUFFER, render_buffer_);

    GLvoid* p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, &((*spheres_)[0]), sizeof(Sphere) * spheres_->size());
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    GLuint binding_point_index = 0;
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_point_index, render_buffer_);

    GLuint spheres = glGetUniformBlockIndex(shader_, "spheres");
    glUniformBlockBinding(shader_, spheres, binding_point_index);    

    GLuint numspheres = glGetUniformLocation(shader_, "numspheres");
    glUniform1i(numspheres, spheres_->size());

    GLuint whratio = glGetUniformLocation(shader_, "whratio");
    glUniform1f(whratio, ((float) w_) / ((float) h_));
}

void Scene::drawGL() {
    GLuint t = glGetUniformLocation(shader_, "t");
    glUniform1i(t, ++t_);

    GLuint seed = glGetUniformLocation(shader_, "user_seed");
    float rand_val = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
    while (rand_val < .2 && rand_val > -.2)
	rand_val = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
    glUniform1f(seed, rand_val);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(frameShape) / 3 / sizeof(float));
}


