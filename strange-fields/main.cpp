// Karl Yerkes
// 2026-05-26
//
// from this...
//   https://w2.mat.ucsb.edu/l.putnam/wrapture/index.html
//
#include <iostream>

#include "Gamma/scl.h"
#include "al/app/al_App.hpp" // al::App
#include "al/app/al_GUIDomain.hpp"
#include "al/io/al_Window.hpp"
#include "al/math/al_Mat.hpp"
#include "al/math/al_Random.hpp"

#include <fstream>
#include <vector>

std::string slurp(std::string fileName) {
  std::fstream file(fileName);
  std::string returnValue = "";
  while (file.good()) {
    std::string line;
    getline(file, line);
    returnValue += line + "\n";
  }
  return returnValue;
}

using namespace al;

class Mat5d : public Mat<5, double> {
public:
  void coefficients(int seed) {
    rnd::Random<> rng;
    rng.seed(seed);
    for (int k = 0; k < size(); k++) {
      mElems[k] = rng.uniformS(); // (-1, 1)
    }
    mElems[4] = mElems[9] = mElems[14] = mElems[19] = 0;
    mElems[24] = 1;
    // yeah. it is column-major for some reason. why?
  }
};

// seed everything to keep determinism
//
double wrapture(std::vector<Vec4d> &v, double b, int seed, int count) {
  Mat5d m;
  m.coefficients(seed);
  // printf("det:%lf\n", determinant(m));

  rnd::Random<> rng;
  rng.seed(seed);
  Vec5d x(rng.ball<Vec4d>(), 1);
  v.push_back(x.elems());

  for (int i = 0; i < count; i++) {
    // linear step
    x = m * x;

    // non-linear step
    for (int k = 0; k < x.size() - 1; k++) {
      x.elems()[k] = gam::scl::wrap(x.elems()[k], b, -b);
    }

    v.push_back(x.elems()); // note: we drop the 5th element here!
  }
  return determinant(m);
}

struct MyApp : public App {
  Parameter b{"b", 1.0, 0.001, 5};
  Parameter r{"r", 0.01, 0.001, 0.031};
  ShaderProgram shader;

  void onInit() override {
    auto GUIdomain = GUIDomain::enableGUI(defaultWindowDomain());
    auto &gui = GUIdomain->newGUI();
    gui.add(b);
    gui.add(r);
  }

  std::vector<Vec4d> v;
  void onCreate() override {
    shader.compile(slurp("../vertex.glsl"), slurp("../fragment.glsl"),
                   slurp("../geometry.glsl"));
  }

  int seed = 0;
  bool onKeyDown(const Keyboard& k) override {
    if (k.key() == ' ') {
      seed++;
      rebuild();
    }
    else if (k.key() == Keyboard::BACKSPACE) {
      seed--;
      rebuild();
    }
    return true;
  }

  bool dragged = true;
  bool onMouseDrag(const Mouse &m) override {
    dragged = true;
    return true;
  }

  void rebuild() {
      v.clear();
      double det = wrapture(v, b, seed, 10000);

      double mean = 0;
      double minimum = 1e100;
      double maximum = -1e100;
      for (auto &e : v) {
        if (e.w > maximum)
          maximum = e.w;
        if (e.w < minimum)
          minimum = e.w;
        mean += e.w;
      }
      mean /= v.size();

      double dev = 0;
      for (auto &e : v) {
        dev += pow(e.w - mean, 2);
      }
      dev = sqrt(dev / v.size());
      printf("det:%lf b:%lf min:%lf max:%lf mean:%lf dev:%lf\n", det, b.get(), minimum, maximum, mean, dev);
  }

  double angle = 0;
  void onAnimate(double dt) override {
    angle += 0.07;
    if (dragged) {
      dragged = false;
      rebuild();
    }
  }

  void onDraw(Graphics &g) override {
    g.shader(shader);
    g.shader().uniform("b", b);
    g.shader().uniform("r", r);
    g.clear(0.27);
    g.rotate(angle, 0, 1, 0);
    g.scale(1 / b);
    g.scale(0.2);
    Mesh m{Mesh::POINTS};
    for (auto &e : v) {
      m.vertex(e);
    }
    g.draw(m);
  }
};

int main() { MyApp().start(); }
