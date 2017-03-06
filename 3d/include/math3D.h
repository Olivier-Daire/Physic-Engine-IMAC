#ifndef MATH3X_H
#define MATH3X_H

#include <g3x.h>

typedef struct {
    double x, y, z;
} Vector3;

Vector3* ProdVec3(Vector3* v, double s);

Vector3* DivVec3(Vector3* v, double s);
Vector3* SubVec3(Vector3* v, double s);
Vector3* AddVec3(Vector3* v, double s);
Vector3 Vec3ProdVec3(Vector3 v, Vector3 v2);
Vector3 Vec3DivVec3(Vector3 v, Vector3 v2);
Vector3 Vec3SubVec3(Vector3 v, Vector3 v2);
Vector3 Vec3AddVec3(Vector3 v, Vector3 v2);
double Distance3(Vector3 v, Vector3 v2);

#endif
