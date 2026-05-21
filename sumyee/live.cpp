// SPDX-FileCopyrightText: 2025 AlloSphere Research Group <allosphere@ucsb.edu>
// SPDX-License-Identifier: BSD-3-Clause
#include "al/app/al_App.hpp"
#include "al/graphics/al_Shader.hpp"
#include "al/graphics/al_VAOMesh.hpp"
#include "al/io/al_File.hpp"
#include <fstream>

using namespace al;

struct MyApp : App {
  VAOMesh quad;
  ShaderProgram *shader;
  float mouseX, mouseY;

  double time{0.};

  bool load(ShaderProgram *p) {
    SearchPaths sp;
    sp.addSearchPath(".", false);
    sp.addAppPaths();
    sp.addRelativePath("..", true);
    auto vertex = sp.find("vertex.glsl");
    if (vertex.valid()) {
      auto fragment = sp.find("fragment.glsl");
      if (fragment.valid()) {
        return p->compile(slurp(vertex.filepath()), slurp(fragment.filepath()));
      }
    }
    return false;
  }

  void onCreate() override {
    shader = new ShaderProgram();
    assert(load(shader));

    lens().near(0.1).far(25).fovy(45);
    nav().pos(0, 0, 4);

    quad.primitive(Mesh::TRIANGLE_STRIP);
    quad.vertex(-1.f, -1.f, 0);
    quad.vertex(1.f, -1.f, 0);
    quad.vertex(-1.f, 1.f, 0);
    quad.vertex(1.f, 1.f, 0);
    quad.texCoord(0, 0);
    quad.texCoord(1, 0);
    quad.texCoord(0, 1);
    quad.texCoord(1, 1);
    quad.update();
  }

  double t = 0;
  void onAnimate(double dt) override {
    time += dt;

    t += dt;
    if (t > 0.1) {
      t -= 0.1;
      auto *p = new ShaderProgram();
      if (load(p)) {
        delete shader;
        shader = p;
      }
    }
  }

  void onDraw(Graphics &g) override {
    g.clear();

    shader->use();
    shader->uniform("iResolution", (float)fbWidth(), (float)fbHeight(),
                   (float)width() / height());
    shader->uniform("iTime", time);
    shader->uniform("iMouse", mouseX, mouseY);
    shader->uniform("eye_sep", g.lens().eyeSep() * g.eye() / 2.0f);
    shader->uniform("foc_len", g.lens().focalLength());
    shader->uniform("al_ProjMatrixInv", Matrix4f::inverse(g.projMatrix()));
    shader->uniform("al_ViewMatrixInv", Matrix4f::inverse(g.viewMatrix()));
    shader->uniform("al_ModelMatrixInv", Matrix4f::inverse(g.modelMatrix()));

    // bypassing graphics class with direct rendering calls
    // no projection involved
    quad.draw();
  }

  bool onMouseMove(const Mouse &m) override {
    mouseX = m.x();
    mouseY = m.y();
    return true;
  }

  std::string slurp(std::string fileName) {
    std::fstream file(fileName);

    // This new part checks if the file actually exists and opened correctly!
    if (!file.is_open()) {
      std::cout << "ERROR: Could not open file: " << fileName << std::endl;
      return "";
    }

    std::string returnValue = "";
    while (file.good()) {
      std::string line;
      getline(file, line);
      returnValue += line + "\n";
    }
    return returnValue;
  }
};

int main() {
  MyApp app;
  app.dimensions(600, 400);
  app.start();
}
