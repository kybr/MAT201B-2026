#include "al/app/al_App.hpp" // al::App
#include <iostream>


class Timer {
    double value = 0;
    double increment = 0;
public:

    void hertz(double hertz, double frameRate) {
        increment = hertz / frameRate;
    }

    bool fired() {
        bool hasFired = false;
        if (value > 1.0) {
            value -= 1.0;
            hasFired = true;
        }
        value += increment;
        return hasFired;
    }
};

struct MyApp : public al::App {
    al::Mesh mesh;

    Timer timer;

    double t = 0;
    int frameCount = 0;
    double ellapsed_time = 0;
    bool shouldClick = false;
    bool shouldFlash = false;

    void onCreate() override {

        timer.hertz(10, 60);


        mesh.primitive(al::Mesh::TRIANGLES);
        mesh.vertex(-1, 0, -2);
        mesh.vertex(0, 1, -2);
        mesh.vertex(1, 0, -2);
        mesh.color(1, 0, 0);
        mesh.color(0, 1, 0);
        mesh.color(0, 0, 1);

        nav().pos(0, 0, 6);
        lens().far(5);
    }

    void onAnimate(double dt) override {
        shouldClick = timer.fired();
        shouldFlash = shouldClick;

        t += 0.01;
        ellapsed_time += dt;
        frameCount++;

        if (ellapsed_time > 1) {
           ellapsed_time -= 1;

           printf("%d\n", frameCount);
           frameCount = 0;
        }
    }

    // running at about 60 Hz
    //
    void onDraw(al::Graphics& g) override {
        g.clear(shouldFlash ? 1.0 : 0.0);
        shouldFlash = false;

        //g.clear(t - (int)t, 0, 0);
        g.meshColor(); // bind of of the allolib shaders
        g.draw(mesh);
        g.color(0, 1, 1); // bind of of the allolib shaders
        g.translate(2, 2, 2);
        g.draw(mesh);
    }

    // running at about 94 Hz
    //
    void onSound(al::AudioIOData& io) override {
        io(); // advance to the 0 sample
        float s = shouldClick ? 1.0 : 0.0;
        shouldClick = false;
        io.out(0) = s; // left
        io.out(1) = s; // right

        while (io()) {
            float s = 0;
            io.out(0) = s; // left
            io.out(1) = s; // right
        }
    }
};

int main() {
    MyApp app;
    app.configureAudio(48000, 512, 2, 2);
    app.start();
}
