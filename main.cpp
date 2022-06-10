#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <windows.h>

#include <vector>
#include <cassert>

#include "render.h"
#include "AntiAliasing.h"

// ------------------------

void prepare_bodies(std::vector<std::shared_ptr<Body>>& bodies) 
{
    auto* geom = (Geometry*) new Sphere({-25,0,10}, 24);
    auto* mat = new Material();
    mat->matte = Vec3f{ 1,0,0 };
    bodies.emplace_back(new Body(geom, mat));

    geom = (Geometry*) new Sphere({25,0,10}, 24);
    mat = new Material();
    mat->matte = Color{ 1,0,0 };
    bodies.emplace_back(new Body(geom, mat));

    geom = (Geometry*) new Sphere({0,0,0}, 28);
    mat = new Material();
    mat->matte = Color{ 0.2,0.2,1 };
    bodies.emplace_back(new Body(geom, mat));
}

void prepare_lights(std::vector<std::shared_ptr<Light>>& lights)
{
    auto* light = new PointLight();
    light->loc = { 50, -80, -5 };
    light->color = { 1, 1, 1 };
    light->brightness = 1;

    lights.emplace_back(light);
}

int main()
{
//    test1();

//    Vec3f v{-100,0,100};
//    std::cout << clip3f(v, -1, 1) << std::endl;
//    return 0;

    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    prepare_bodies(scene->bodies);
    prepare_lights(scene->lights);

    Renderer renderer;
    renderer.scene = scene.get();

    renderer.width = 800;
    renderer.height = 600;
    renderer.fov = 75;
    renderer.camera = { {0, -120, 0}, {0, 1, 0} };
    renderer.camera_up = { 0, 0, 1 };
    renderer.camera_right = { 1, 0, 0 };

    renderer.backgroundColor = Color{0,0,0};
    renderer.globalIllumination = Color{1,1,1} * 0.05;

    std::cout << "Starting the render..." << std::endl;
    renderer.Render();
    std::cout << "Render finished!" << std::endl;

    BMP bmp(renderer.width, renderer.height, renderer.GetBuffer());
    bmp.write("output.bmp");

//    UniformSSAAx4 ssaa(&renderer);
//    ssaa.Apply();
//
//    bmp = BMP(ssaa.GetWidth(), ssaa.GetHeight(), ssaa.GetOutput());
//    bmp.write("output-aa.bmp");
//
//    AverageFilter avgFilter(&renderer, 3);
//    avgFilter.Apply();
//
//    bmp = BMP(avgFilter.GetWidth(), avgFilter.GetHeight(), avgFilter.GetOutput());
//    bmp.write("output-avg3.bmp");


    return 0;
}
