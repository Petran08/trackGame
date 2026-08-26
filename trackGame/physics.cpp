#include "physics.h"
#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

bool CheckCollisionSphereMesh(Vector3 center, float radius, Vector3 carPos,  Mesh mesh, Matrix transform, Vector3& normals, float& pushDist)
{
	bool collided = false;

	Vector3 closestPoint = { 0.0f };//to put in the triangle sphere collision

	Ray col;//used to calculate the normal
	col.position = center;

	float minAngle = 180 * DEG2RAD;//to see which normal should be the one that pushes u back
	float maxAlignment = 0;

	if (mesh.indices != NULL)//if the mesh uses the indices array
	{
		for (int i = 0; i < mesh.triangleCount; i++) {//for every triangle
			int i0 = mesh.indices[i * 3 + 0];//take the indices for the triangles
			int i1 = mesh.indices[i * 3 + 1];
			int i2 = mesh.indices[i * 3 + 2];

			Vector3 a = { mesh.vertices[i0 * 3 + 0], mesh.vertices[i0 * 3 + 1], mesh.vertices[i0 * 3 + 2] };//take the veritces
			Vector3 b = { mesh.vertices[i1 * 3 + 0], mesh.vertices[i1 * 3 + 1], mesh.vertices[i1 * 3 + 2] };
			Vector3 c = { mesh.vertices[i2 * 3 + 0], mesh.vertices[i2 * 3 + 1], mesh.vertices[i2 * 3 + 2] };

			a = Vector3Transform(a, transform);
			b = Vector3Transform(b, transform);
			c = Vector3Transform(c, transform);

			if (checkCollisionSphereTriangle(center, radius, a, b, c, closestPoint))//check collision
			{
				collided = true;//collided
				col.direction = Vector3Normalize(Vector3Subtract(closestPoint, center));//have the direction to the closest point(normalized)
				Vector3 normal = triangleNormal(a, b, c);//take the normal
				
				Vector3 pushDirection = Vector3Subtract(
					Vector3Zero(),
					col.direction
				);
				// make the normal point in the push direction
				if (Vector3DotProduct(normal, pushDirection) < 0.0f)
				{
					normal = Vector3Subtract(Vector3Zero(), normal);
				}

				// keep the normal most aligned with the push direction
				float alignment = Vector3DotProduct(normal, pushDirection);

				if (alignment > maxAlignment)
				{
					normals = normal;
					maxAlignment = alignment;
					pushDist = abs(radius - Vector3Distance(center, closestPoint));
				}
			}
		}

	}
	else//if the mesh doesn`t use indices
	{
		for (int i = 0; i < mesh.triangleCount; i++) {
			int base = i * 9; // take the indices in order for every triangle
			Vector3 a = { mesh.vertices[base + 0], mesh.vertices[base + 1], mesh.vertices[base + 2] };//do the same things as before
			Vector3 b = { mesh.vertices[base + 3], mesh.vertices[base + 4], mesh.vertices[base + 5] };//but the vertices are calculated slightly different
			Vector3 c = { mesh.vertices[base + 6], mesh.vertices[base + 7], mesh.vertices[base + 8] };



			if (checkCollisionSphereTriangle(center, radius, a, b, c, closestPoint))
			{
				collided = true;
				col.direction = Vector3Normalize(Vector3Subtract(closestPoint, center));
				Vector3 normal = triangleNormal(a, b, c);
				
				Vector3 pushDirection = Vector3Subtract(
					Vector3Zero(),
					col.direction
				);

				// make the normal point in the push direction
				if (Vector3DotProduct(normal, pushDirection) < 0.0f)
				{
					normal = Vector3Subtract(Vector3Zero(), normal);
				}

				// keep the normal most aligned with the push direction
				float alignment = Vector3DotProduct(normal, pushDirection);

				if (alignment > maxAlignment)
				{
					normals = normal;
					maxAlignment = alignment;
					pushDist = abs(radius - Vector3Distance(center, closestPoint));
				}
			}
		}

	}

	return collided;//return if collided
}


Vector3 closestPointTriangle(Vector3 const& p, Vector3 const& a, Vector3 const& b, Vector3 const& c)
{
	//i`ve got this code somewhere on the internet
	//i`ll leave the link for it. idk how it works but it works
	//https://github.com/embree/embree/blob/master/tutorials/common/math/closest_point.h
	//thanks embree for this function, i only needed to adapt it to raylib code

	const Vector3 ab = b - a;
	const Vector3 ac = c - a;
	const Vector3 ap = p - a;

	const float d1 = Vector3DotProduct(ab, ap);
	const float d2 = Vector3DotProduct(ac, ap);
	if (d1 <= 0.f && d2 <= 0.f) return a; //#1

	const Vector3 bp = p - b;
	const float d3 = Vector3DotProduct(ab, bp);
	const float d4 = Vector3DotProduct(ac, bp);
	if (d3 >= 0.f && d4 <= d3) return b; //#2

	const Vector3 cp = p - c;
	const float d5 = Vector3DotProduct(ab, cp);
	const float d6 = Vector3DotProduct(ac, cp);
	if (d6 >= 0.f && d5 <= d6) return c; //#3

	const float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
	{
		const float v = d1 / (d1 - d3);
		return a + Vector3Scale(ab, v); //#4
	}

	const float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f)
	{
		const float v = d2 / (d2 - d6);
		return a + Vector3Scale(ac, v); //#5
	}

	const float va = d3 * d6 - d5 * d4;
	if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f)
	{
		const float v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + Vector3Scale((c - b), v); //#6
	}

	const float denom = 1.f / (va + vb + vc);
	const float v = vb * denom;
	const float w = vc * denom;
	return a + Vector3Scale(ab, v) + Vector3Scale(ac, w); //#0
}

bool checkCollisionSphereTriangle(Vector3 center, float radius, Vector3 p1, Vector3 p2, Vector3 p3, Vector3& closestPoint)
{
	closestPoint = closestPointTriangle(center, p1, p2, p3); //we get the closest point(we return it in a parameter for later use)
	if (Vector3Distance(center, closestPoint) <= radius)//if the closest point is inside the sphere we got a hit
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector3 triangleNormal(Vector3 p1, Vector3 p2, Vector3 p3)//just triangle normal
{
	Vector3 a = p2 - p1;
	Vector3 b = p3 - p1;
	return Vector3Normalize(Vector3CrossProduct(a, b));
}

bool VectorsFaceSameDirection(Vector3 a, Vector3 b, float maxAngleDeg)//verify if they face the same direction(aprox)
{
	float dot = Vector3DotProduct(a, b);
	float threshold = cosf(DEG2RAD * maxAngleDeg);
	return dot > threshold;
}