#include "al/app/al_App.hpp" // al::App
#include "al/app/al_GUIDomain.hpp"
#include <fstream>

using namespace al;
using namespace std;

string slurp(string fileName);

struct MyApp : public App {
  ShaderProgram shader;
  FBO fbo;
  RBO rbo;
  Texture tex;

  void updateFBO(int w, int h) {
    tex.create2D(w, h);
    rbo.resize(w, h);
    fbo.bind();
    fbo.attachTexture2D(tex);
    fbo.attachRBO(rbo);
    fbo.unbind();
  }

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    // gui.add(pointSize);  // add parameter to GUI
    //
  }
  void onCreate() override {
    shader.compile(slurp("../vertex.glsl"), slurp("../fragment.glsl"));
    updateFBO(width(), height());
  }

  void onResize(int w, int h) override { updateFBO(w, h); }

  double t = 0;
  
  double time = 0;
  void onAnimate(double dt) override {
    time += dt;
    t += dt;
    if (t > 0.25) {
        t -= 0.25;
        // reload shader
        shader.compile(slurp("../vertex.glsl"), slurp("../fragment.glsl"));
    }
  }

  void onDraw(al::Graphics &g) override {
    fbo.bind();
    g.viewport(0, 0, tex.width(), tex.height());
    g.clear(0, 0, 0);

    g.shader(shader);
    Vec3f r(width(), height(), (float)width() / height());
    g.shader().uniform("iResolution", (float)width(), (float)height(), (float)width() / height());
    g.shader().uniform("iTime", time);
    g.shader().uniform("iMouse", mouseX, mouseY);
    //g.shader().uniform("iResolution", r);
    Mesh m;
    m.vertex(-1, -1);
    m.vertex(1, -1);
    m.vertex(1, 1);
    m.vertex(-1, -1);
    m.vertex(1, 1);
    m.vertex(-1, 1);
    g.draw(m);

    fbo.unbind();

    //g.clear(1, 1, 1);
    //g.viewport(0, 0, fbWidth(), fbHeight());
    g.quadViewport(tex, -1, -1, 2, 2);
  }

  float mouseX, mouseY;
  bool onMouseMove(const Mouse& m) override {
    mouseX = m.x();
    mouseY = m.y();
    return true;
  }
};

int main() {
  MyApp app;
  app.start();
}

string slurp(string fileName) {
  fstream file(fileName);
  string returnValue = "";
  while (file.good()) {
    string line;
    getline(file, line);
    returnValue += line + "\n";
  }
  return returnValue;
}