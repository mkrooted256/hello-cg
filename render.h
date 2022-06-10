#pragma once

#include "scene.h"
#include "bmp.h"
#include <cmath>

struct Renderer {
    Scene* scene;
    Tracer tracer;

    int width;
    int height;
    float fov;
    Ray camera;
    Vec3f camera_up;
    Vec3f camera_right;

    Color backgroundColor = {0,0,0};
    Color globalIllumination = { 0,0,0};

private:
    std::vector<Color> img;
public:

    Color GetPointColor(const TraceResult& trace, Body * body) {
        Color visibleColor = { 0, 0, 0};

        for (const std::shared_ptr<Light>& light : scene->lights) {
            auto lightTestRays = light->GetTestRays(trace.hitpoint);
            for (const Ray& r : lightTestRays) {
                TraceResult lightTraceResult{};
                auto lightHitBody = tracer.Trace(scene, r, lightTraceResult);
                if (!light->IsRayBlocked(lightTraceResult, trace.geom)) {
                    const Color illumination = light->Illuminate(trace.hitpoint, trace.normal);
                    visibleColor += body->mat->GetSurfaceColor(illumination, trace.normal, camera.origin - trace.hitpoint);
                }
            }
        }
        visibleColor += body->mat->emission * body->mat->brightness;
        visibleColor += globalIllumination;

        // TODO: handle other bodies' emission
        return visibleColor;
    }

    virtual void Render() {
        const float tanHalfFov = tan(M_PI * fov / 360.f);
        const float aspect = width / float(height);
        const float D = width / (2 * tanHalfFov);

        /*
         * Note: direction is important.
         *   We start at lower left corner.
         *   This is how BMP works -
         */
        const Vec3f verticalStep = -camera_up;
        const Vec3f horizontalStep = camera_right;

        Vec3f pixel = D * camera.dir;
        pixel += (height/2.f) * camera_up;
        pixel -= (width/2.f) * camera_right;

        Vec3f pixelRowStart = pixel;

        std::cout << "rendering : D = " << D << std::endl;

        const int p = height/10;
        int next_checkpoint = p;

        unsigned int n = height * width;
        img.reserve(n);
        n = 0;

        Ray ray{};
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                ray.origin = camera.origin;
                ray.dir = pixel.Ort();

                TraceResult traceResult;
                Body* hitBody = tracer.Trace(scene, ray, traceResult);

                Color visibleColor = backgroundColor;
                if (hitBody)
                    visibleColor = GetPointColor(traceResult, hitBody);

                visibleColor = clip3f(visibleColor, 0, 1);

                img.push_back(visibleColor);

                pixel += horizontalStep;
            }
            pixelRowStart += verticalStep;
            pixel = pixelRowStart;

            if (y == next_checkpoint) {
                next_checkpoint += p;
                std::cout << "rendering : " << y << "/" << height << std::endl;
            }
        }
        std::cout << "first pass : finished" << std::endl;
    }

    virtual const std::vector<Color>& GetBuffer() { return img; }
};

struct PostRenderer {
protected:
    std::vector<Color> output;
    Renderer * renderer;
public:
    explicit PostRenderer(Renderer  * renderer) : renderer(renderer) {};

    virtual void Apply() = 0;

    const std::vector<Color>& GetOutput() { return output; }
    virtual size_t GetWidth() = 0;
    virtual size_t GetHeight() = 0;
};
