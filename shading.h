#pragma once

#include "common.h"
#include "scene.h"
#include <vector>

#define EPS (1e-6)
#define is_smol(f) ((-EPS < f) && (f < EPS))


// ----- SHADING -----

struct Material {
    Color matte = {0.5, 0.5, 0.5};
    Color emission = {0,0,0};

    float brightness = 0;
    float transparency = 0;
    float reflection = 0;

    // toCam is for specular
    virtual Color GetSurfaceColor(Color illumination, Vec3f normal, Vec3f toCam) {
        return illumination.Multiply( matte  );
    }
};
