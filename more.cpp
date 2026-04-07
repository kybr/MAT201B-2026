#include <iostream>

#include "al/app/al_App.hpp"  // al::App
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"

using namespace al;

struct MyApp : public App {
  Light light;        // Necessary to light objects in the scene
  Material material;  // Necessary for specular highlights
  Mesh mesh;
  Quatd target;
  double t = 0;
  Quatd orientation;

  void onCreate() override {
    addCone(mesh);
    mesh.scale(1, 0.2, 1);
    mesh.generateNormals();

    nav().pos(0, 0, 6);
    light.pos(-2, 7, 0);
  }

  void onAnimate(double dt) override {
    t += dt;
    if (t > 2) {
      t -= 2;
      target =
          Quatd(rnd::uniform(), rnd::uniform(), rnd::uniform(), rnd::uniform())
              .normalize();
    }

    orientation.slerpTo(target, 0.03);
  }

  void onDraw(Graphics& g) override {
    g.clear(0.27);

    light.ambient(RGB(0));          // Ambient reflection for this light
    light.diffuse(RGB(1, 1, 0.5));  // Light scattered directly from light
    g.lighting(true);
    g.light(light);
    material.specular(light.diffuse() * 0.2);  // Specular highlight, "shine"
    material.shininess(50);  // Concentration of specular component [0,128]

    g.material(material);

    g.rotate(orientation);
    g.draw(mesh);
  }
};

int main() { MyApp().start(); }
