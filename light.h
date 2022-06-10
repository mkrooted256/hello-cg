#pragma once

#include "geometry.h"

// ----- LIGHT -----

struct Light {
    virtual Color Illuminate(const Vec3f& p, const Vec3f& normal) = 0;
    virtual std::vector<Ray> GetTestRays(const Vec3f& p) = 0;
    virtual bool IsRayBlocked(const TraceResult& traceResult, Geometry* illuminatedGeom) = 0;
};

struct PointLight : public Light {
    Vec3f loc = {0,0,0};
    Color color = {1,1,1};
    float brightness = 1;

    Color Illuminate(const Vec3f& p, const Vec3f& normal) override {
        float cos = normal.Cos(loc - p);
        if (cos < 0) return {0,0,0};
        return color * cos * brightness;
    }

    std::vector<Ray> GetTestRays(const Vec3f &p) override {
        return { Ray{loc, (p-loc).Ort() } };
    }

    bool IsRayBlocked(const TraceResult &traceResult, Geometry* illuminatedGeom) override {
        return traceResult.geom != illuminatedGeom;
    }
};

struct DirectionalLight : public Light {
    Vec3f dir = {0,0,-1};
    Color color = {1,1,1};
    float brightness = 1;

    Color Illuminate(const Vec3f& p, const Vec3f& normal) override {
        float cos = normal.Cos(dir);
        if (cos < 0) return {0,0,0};
        return color * cos * brightness;
    }

    std::vector<Ray> GetTestRays(const Vec3f &p) override {
        return { Ray{p, -dir } };
    }

    bool IsRayBlocked(const TraceResult &traceResult, Geometry *illuminatedGeom) override {
        return !traceResult.geom;
    }
};