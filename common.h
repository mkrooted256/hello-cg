#pragma once

// ----- MACRO -----

#define clip(v, min, max) ( (v<min) ? (min) : ( (v>max) ? max : v ) )
#define lerp(a, b, x) (a*x + b*(1-x))

// ----- VECTOR -----

struct Vec3f {
    float x;
    float y;
    float z;

    Vec3f operator*(float k) const { return { x*k, y*k, z*k }; }
    friend Vec3f operator*(float k, const Vec3f& v) { return v*k; }
    Vec3f operator/(float k) const { return { x/k, y/k, z/k }; }

    Vec3f& operator+=(const Vec3f& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
    Vec3f& operator-=(const Vec3f& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
    Vec3f operator+(const Vec3f& v) const { return { x+v.x, y+v.y, z+v.z }; }
    Vec3f operator-(const Vec3f& v) const { return { x-v.x, y-v.y, z-v.z };; }
    Vec3f operator-() const { return { -x, -y, -z }; }

    float Dot(const Vec3f& v) const { return x*v.x + y*v.y + z*v.z; }
    Vec3f Cross(const Vec3f& v) const { return { y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x }; }

    float Normsq() const { return x*x + y*y + z*z; };
    float Norm() const { return sqrt(x*x + y*y + z*z); };
    Vec3f Ort() const { return *this / Norm(); } // TODO: fast inverse sqrt

    float Cos(const Vec3f& v) const { return Dot(v) / sqrt(Normsq() * v.Normsq()); }

    bool operator>(float f) const { return x>f && y>f && z>f; }
    bool operator>=(float f) const { return x>=f && y>=f && z>=f; }
    bool operator<(float f) const { return x<f && y<f && z<f; }
    bool operator<=(float f) const { return x<=f && y<=f && z<=f; }

    Vec3f Reflect(const Vec3f& normal) const { return *this + (normal + normal) * this->Dot(normal); }
    
    Vec3f Multiply(const Vec3f& v) const { return { x*v.x, y*v.y, z*v.z }; }

    friend std::ostream& operator<<(std::ostream& os, const Vec3f& vec) {
        os << "( " << vec.x << " ; " << vec.y << " ; " << vec.z << ")";
        return os;
    } 
};

#define clip3f(vec, min, max) (Vec3f{ clip((vec).x, min, max), clip((vec).y, min, max), clip((vec).z, min, max) })

struct Vec2f {
    float x;
    float y;
    
    Vec2f operator*(float k) const { return { x*k, y*k }; }
    friend Vec2f operator*(float k, const Vec2f& v) { return v*k; }
    Vec2f operator/(float k) const { return { x/k, y/k }; }

    Vec2f& operator+=(const Vec2f& v) { x+=v.x; y+=v.y; return *this; }
    Vec2f& operator-=(const Vec2f& v) { x-=v.x; y-=v.y; return *this; }
    Vec2f operator+(const Vec2f& v) const { return { x+v.x, y+v.y }; }
    Vec2f operator-(const Vec2f& v) const { return { x-v.x, y-v.y };; }
    Vec2f operator-() const { return { -x, -y }; }

    float Dot(const Vec2f& v) const { return x*v.x + y*v.y; }

    float Normsq() const { return x*x + y*y; };
    float Norm() const { return sqrt(x*x + y*y); };
    Vec2f Ort() const { return *this / Norm(); } // TODO: fast inverse sqrt

    float Cos(const Vec2f& v) const { return Dot(v) / sqrt(Normsq() * v.Normsq()); }

    bool operator>(float f) const { return x>f && y>f; }
    bool operator>=(float f) const { return x>=f && y>=f; }
    bool operator<(float f) const { return x<f && y<f; }
    bool operator<=(float f) const { return x<=f && y<=f; }

    Vec2f Reflect(const Vec2f& normal) const { return *this + (normal + normal) * this->Dot(normal); }

    Vec2f Multiply(const Vec2f& v) const { return { x*v.x, y*v.y }; }

    friend std::ostream& operator<<(std::ostream& os, const Vec2f& vec) {
        os << "( " << vec.x << " ; " << vec.y << " )";
        return os;
    }

};

// ----- COLOR -----

typedef Vec3f Color;
