#pragma once
#include <algorithm>
#include <array>
#include "math.h"

const float RADIANS = 57.2958f;
enum class stateMouse {IDLE = 0U, CLICK = 1U, CLICK_OUT = 2U};
static float valueEmpty = 0.0f;

template <size_t countVec>
struct vec {
    std::array<float, countVec> value;
    
    float& x() {
        return (*this)[0];
    }
    
    float& y() {
        return (*this)[1];
    }
    
    float& z() {
        return (*this)[2];
    }
    
    float& w() {
        return (*this)[3];
    }

    vec() {
        for (size_t n = 0; n < countVec; n++) {
            value[n] = 0.f;
        }
    }
    
    vec(float aX, float aY) {
        value[0] = aX;
        value[1] = aY;
    }
    
    vec(float aX, float aY, float aZ) {
        value[0] = aX;
        value[1] = aY;
        value[2] = aZ;
    }
    
    vec(float aX, float aY, float aZ, float aW) {
        value[0] = aX;
        value[1] = aY;
        value[2] = aZ;
        value[3] = aW;
    }
    
    template<typename T>
    vec(const T& aValue) {
        for (size_t n = 0U; n < aValue.size(); n++) {
            value[n] = aValue[n];
        }
    }
    
    size_t size() const{
        return countVec;
    }
    
    float& operator[] (size_t idx) {
        if (idx > countVec) {
            return valueEmpty;
        }
        return value[idx];
    }
    
    float operator[] (size_t idx) const {
        if (idx > countVec) {
            return valueEmpty;
        }
        return value[idx];
    }
    
    template <typename T>
    float& operator[] (T idx) {
        return value[static_cast<size_t>(idx)];
    }
    
    float getModule() {
        float result = 0.f;
        for (size_t n = 0U; n < countVec; n++) {
            result += value[n] * value[n];
        }
        return sqrtf(result);
    }
    
    static float getScalarProduct(const vec& a, const vec& b) {
        float result = 0.f;
        for (size_t n = 0U; n < countVec; n++) {
            result += a[n] * b[n];
        }
        return result;
    }
    
    static float getAngle(const vec& a, const vec& b) {
        return acosf(a.getModule() * b.getModule() / getScalarProduct(a, b));
    }
    
    static float getVectorProduct(const vec& a, const vec& b) {
        return a.getModule() * b.getModule() * sinf(getAngle(a, b));
    }
    
    vec operator* (const vec& aValue) {
        vec result;
        for (size_t n = 0U; n < countVec; n++) {
            result[n] = value[n] * aValue[n];
        }
        return result;
    }
    
    vec operator+ (const vec& aValue) {
        vec result;
        for (size_t n = 0U; n < countVec; n++) {
            result[n] = value[n] + aValue[n];
        }
        return result;
    }
    
    vec operator- (const vec& aValue) {
        vec result;
        for (size_t n = 0U; n < countVec; n++) {
            result[n] = value[n] - aValue[n];
        }
        return result;
    }
    
    vec& operator= (const vec& aValue) {
        for (size_t n = 0U; n < countVec; n++) {
            value[n] = aValue[n];
        }
        return *this;
    }
};

template <size_t countVec>
vec<countVec> operator* (vec<countVec> a, vec<countVec> b) {
    vec<countVec> result;
    for (size_t n = 0U; n < countVec; n++) {
        result[n] = a[n] * b[n];
    }
    return result;
}

using vec4f = vec<4U>;
using vec3f = vec<3U>;
using vec2f = vec<2U>;

struct mat4f {
    float value[16];
    float& operator [] (size_t i) {
        return value[i];
    }
    
    static vec3f matToVec3f(mat4f& mat) {
        vec3f result;
        float& x = result[0];
        float& y = result[1];
        float& z = result[2];
        
        y = -asinf(mat[2]);
        auto cosY = cos(y);
        y *= RADIANS;
        if (fabs(cosY) > 0.005f) {
            auto xTr = mat[10] / cosY;
            auto yTr = -mat[6] / cosY;
            x = atan2(yTr,xTr) * RADIANS;
            xTr = mat[0] / cosY;
            yTr = mat[1] / cosY;
            z = atan2(yTr,xTr) * RADIANS;
        }
        else {
            x = 0;
            auto xTr = mat[5];
            auto yTr = mat[4];
            z = atan2(yTr,xTr) * RADIANS;
        }
        
        x = std::clamp(fabsf(x), 0.f, 360.f );
        y = std::clamp(fabsf(y), 0.f, 360.f );
        z = std::clamp(fabsf(z), 0.f, 360.f );
        return result;
    }
  
    static mat4f eulerToMat3f(float x, float y, float z) {
        mat4f mat;
        x = x / RADIANS;
        y = y / RADIANS;
        z = z / RADIANS;
        auto a = cosf(x);
        auto b = sinf(x);
        auto c = cosf(y);
        auto d = sinf(y);
        auto e = cosf(z);
        auto f = sinf(z);

        auto ad = a*d;
        auto bd = b*d;

        mat[0] = c*e;
        mat[1] = -c*f;
        mat[2] = -d;
        mat[4] = -bd*e+a*f;
        mat[5] =  bd*f+a*e;
        mat[6] =  -b*c;
        mat[8] =  ad*e+b*f;
        mat[9] = -ad*f+b*e;
        mat[10] = a*c;

        mat[3] = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0;
        mat[15] = 1;
        return mat;
    }
    
    float* get() {
        return value;
    }
};

struct quaternion {
    vec4f value;
    const static inline vec3f axisX = vec3f(1.f, 0.f, 0.f);
    const static inline vec3f axisY = vec3f(0.f, 1.f, 0.f);
    const static inline vec3f axisZ = vec3f(0.f, 0.f, 1.f);
    
    float& operator [] (size_t i) {
        return value[i];
    }
    
    float operator [] (size_t i) const {
        return value[i];
    }
    
    quaternion operator* (const quaternion& q) {
        quaternion result;
        auto x1 = value[0];
        auto x2 = q[0];
        auto y1 = value[1];
        auto y2 = q[1];
        auto z1 = value[2];
        auto z2 = q[2];
        auto w1 = value[3];
        auto w2 = q[3];
        result[0] = w1*x2 + x1*w2 + y1*z2 - z1*y2;
        result[1] = w1*y2 - x1*z2 + y1*w2 + z1*x2;
        result[2] = w1*z2 + x1*y2 - y1*x2 + z1*w2;
        result[3] = w1*w2 - x1*x2 - y1*y2 - z1*z2;
        quaternion::normal(result);
        return result;
    }
    
    static void normal(quaternion& q) {
        float length = (q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
        q[0] = q[0] / length;
        q[1] = q[1] / length;
        q[2] = q[2] / length;
        q[3] = q[3] / length;
    }
    
    static quaternion getFromEuler(const vec3f& vector, float angle) {
        quaternion q;
        auto sinAngle = sinf(angle / RADIANS / 2.f);
        auto cosAngle = cosf(angle / RADIANS / 2.f);

        q[0] = vector[0] * sinAngle;
        q[1] = vector[1] * sinAngle;
        q[2] = vector[2] * sinAngle;
        q[3] = cosAngle;
        return q;
    }
    
    static quaternion getFromEuler3(float x, float y, float z) {
        auto qx = getFromEuler(axisX, x);
        auto qy = getFromEuler(axisY, y);
        auto qz = getFromEuler(axisZ, z);
        return qx*qy*qz;
    }
    
    static mat4f convertToMat3f(const quaternion& q) {
        auto xx = q[0]*q[0];
        auto xy = q[0]*q[1];
        auto xz = q[0]*q[2];
        auto xw = q[0]*q[3];
        auto yy = q[1]*q[1];
        auto yz = q[1]*q[2];
        auto yw = q[1]*q[3];
        auto zz = q[2]*q[2];
        auto zw = q[2]*q[3];
    
        mat4f mat;
        mat[0] = 1-2*(yy+zz);
        mat[1] = 2*(xy-zw);
        mat[2] = 2*(xz+yw);

        mat[4] = 2*(xy+zw);
        mat[5] = 1-2*(xx+zz);
        mat[6] = 2*(yz-xw);

        mat[8] = 2*(xz-yw);
        mat[9] = 2*(yz+xw);
        mat[10] = 1-2*(xx+yy);
        
        mat[3] = mat[7] = mat[11] = mat[12] = mat[13] = mat[14] = 0.f;
        mat[15] = 1.f;
        return mat;
    }
    
    static vec3f convertToEuler3f(const quaternion& q) {
        auto mat = convertToMat3f(q);
        return mat4f::matToVec3f(mat);
    }
    
    quaternion& operator= (const quaternion& aValue) {
        value[0] = aValue[0];
        value[1] = aValue[1];
        value[2] = aValue[2];
        value[3] = aValue[3];
        return *this;
    }
};


