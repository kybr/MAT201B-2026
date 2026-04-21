#include <iostream>

#include "al/app/al_App.hpp"  // al::App
#include "al/graphics/al_Shapes.hpp"
#include "al/math/al_Random.hpp"

using namespace al;

struct MyApp : public App {
  Light light;        // Necessary to light objects in the scene
  Material material;  // Necessary for specular highlights
  Mesh mesh;

  void onCreate() override {
    addSphere(mesh, 1, 100, 100);
    mesh.translate(2, 0, 0);
    addSphere(mesh);
    mesh.translate(2, 0, 0);
    addSphere(mesh);
    mesh.translate(2, 0, 0);
    addSphere(mesh);
    mesh.translate(2, 0, 0);
    addSphere(mesh);

    mesh.generateNormals();

    nav().pos(0, 0, 6);
    light.pos(-2, 7, 0);
  }

  void onAnimate(double dt) override {
  }

  void onDraw(Graphics& g) override {
    g.clear(0.27);

    light.ambient(RGB(0));          // Ambient reflection for this light
    light.diffuse(RGB(1, 1, 0.5));  // Light scattered directly from light
    g.lighting(true);
    g.light(light);
    material.specular(light.diffuse() * 0.2);  // Specular highlight, "shine"
    material.shininess(50);

    g.material(material);
    g.draw(mesh);
  }
};

int main() { MyApp().start(); }
