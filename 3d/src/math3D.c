#include "../include/math3D.h"
#include <math.h>

Vector3* ProdVec3(Vector3* v, double s) {
    v->x *= s;
    v->y *= s;
    v->z *= s;
    return v;
}

Vector3* DivVec3(Vector3* v, double s) {
    v->x /= s;
    v->y /= s;
    v->z /= s;
    return v;
}

Vector3* SubVec3(Vector3* v, double s) {
    v->x -= s;
    v->y -= s;
    v->z -= s;
    return v;
}

Vector3* AddVec3(Vector3* v, double s) {
    v->x += s;
    v->y += s;
    v->z += s;
    return v;
}

Vector3 Vec3ProdVec3(Vector3 v, Vector3 v2) {
    Vector3 result;
    result.x = v.x * v2.x;
    result.y = v.y * v2.y;
    result.z = v.z * v2.z;
    return result;
}

Vector3 Vec3DivVec3(Vector3 v, Vector3 v2) {
    Vector3 result;
    result.x = v.x / v2.x;
    result.y = v.y / v2.y;
    result.z = v.z / v.z;
    return result;
}

Vector3 Vec3SubVec3(Vector3 v, Vector3 v2) {
    Vector3 result;
    result.x = v.x - v2.x;
    result.y = v.y - v2.y;
    result.z = v.z - v2.z;
    return result;
}

Vector3 Vec3AddVec3(Vector3 v, Vector3 v2) {
    Vector3 result;
    result.x = v.x + v2.x;
    result.y = v.y + v2.y;
    result.z = v.z + v2.z;
    return result;
}

double Distance3(Vector3 v, Vector3 v2) {
    double sqrx = v2.x - v.x;
    sqrx *= sqrx;

    double sqry = v2.y - v.y;
    sqry *= sqry;

    double sqrz = v2.z - v.z;
    sqrz *= sqrz;

    return sqrt(sqrx + sqry + sqrz);
}
