#pragma once

#include "common.h"

#define M_PI (3.141592653589793)
#define EPS (1e-6)
#define is_smol(f) ((-EPS < f) && (f < EPS))

struct Geometry;

// ----- TRACING -------

struct Ray {
    Vec3f origin;
    Vec3f dir;
};

struct TraceResult {
    Geometry * geom = nullptr;
    Vec3f hitpoint = {0,0,0};
    Vec3f normal = {0,0,0};
    float t = -1;
};

// ----- GEOMETRY -----

struct Geometry {
    // bool doublesided = false;

    /* true if hit */
    virtual bool Intersect(const Ray& ray, TraceResult& result) { return false; };
};


struct Plane : public Geometry {
    Vec3f normal;
    float d;

    /* F(x,y,z) = Ax + By + Cz + d */
    /* F(point) = 0 */
    /* normal.Dot(point) + d = 0 */

    Plane(Vec3f normal, Vec3f point) : normal(normal), d(-normal.Dot(point)) {};

    float F(Vec3f p) const { return normal.Dot(p) + d; }

    bool Intersect(const Ray& ray, TraceResult& result) override {
        /*
        (Aa + Bb + Cc)t + Aa0 + Bb0 + Cc0 + d = 0
        t = -F(p0) / normal.Dot(ray.direction)
        */
        float prod = normal.Dot(ray.dir);
        if ( prod > -EPS ) return false;

        float t = -F(ray.origin) / prod;
        if ( t < 0 ) return false;

        result.hitpoint = ray.origin + ray.dir * t;
        result.normal = normal;
        result.t = t;
        result.geom = this;

        return true;
    }
};

struct Sphere : public Geometry {
    Vec3f O;
    float R;
private:
    float Rsq;
public:

    Sphere(const Vec3f& origin, float radius) : O(origin), R(radius), Rsq(radius*radius) {}

    float F(const Vec3f& p) const { return (p - O).Normsq() - Rsq; }

    bool Intersect(const Ray& ray, TraceResult& result) override {
        /*
         *  l = O - v0
         *  |v t - l|^2 - R^2 = 0
         *  v^2 t^2 - 2 v l t + l^2 - R^2 = 0
         *  (v^2 = 1)
         *  t^2 - 2 v l t + l^2 - R^2 = 0
         *  D4 = (v l)^2 - l^2 + R^2
         *  t = v l - sqrt(D4)
         */


        const Vec3f l = O - ray.origin;
        const float prod = l.Dot(ray.dir);

        const float D = prod * prod - l.Dot(l) + Rsq;
        if (D < 0) return false;

        const float t1 = prod - sqrt(D);
//        const float t2 = prod + sqrt(D);

        if (t1 < 0) return false;

        result.hitpoint = ray.origin + ray.dir * t1;
        result.normal = (result.hitpoint - O).Ort();
        result.t = t1;
        result.geom = this;

        return true;
    }
};
