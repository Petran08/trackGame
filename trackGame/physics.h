#pragma once
#include <raylib.h>

bool CheckCollisionSphereMesh(Vector3 center, float radius, Vector3 carPos, Mesh mesh, Matrix transform, Vector3& normals, float& pushDist);
Vector3 closestPointTriangle(Vector3 const& p, Vector3 const& a, Vector3 const& b, Vector3 const& c);
bool checkCollisionSphereTriangle(Vector3 center, float radius, Vector3 p1, Vector3 p2, Vector3 p3, Vector3& closestPoint);
Vector3 triangleNormal(Vector3 p1, Vector3 p2, Vector3 p3);
bool VectorsFaceSameDirection(Vector3 a, Vector3 b, float maxAngleDeg);
Vector3 ProjectVectorOntoPlane(Vector3 v, Vector3 normal);
float SignedAngle(Vector3 a, Vector3 b, Vector3 axis);