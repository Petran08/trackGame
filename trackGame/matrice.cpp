#include "matrice.h"
#include "raylib.h"

Vector3 rotatePointAroundAxis(Vector3 point, Vector3 axis, float angle)//might go unused
{
	Vector3 rPoint;

	matrice mPoint(3, 1);

	mPoint.values[0][0] = point.x;
	mPoint.values[1][0] = point.y;
	mPoint.values[2][0] = point.z;

	matrice rMatrix(3, 3);

	angle = angle * DEG2RAD;
	//this formula is from wikipedia in the article about rotation matrices
	rMatrix.values[0][0] = axis.x * axis.x * (1 - cos(angle)) + cos(angle);
	rMatrix.values[0][1] = axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle);
	rMatrix.values[0][2] = axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle);
	rMatrix.values[1][0] = axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle);
	rMatrix.values[1][1] = axis.y * axis.y * (1 - cos(angle)) + cos(angle);
	rMatrix.values[1][2] = axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle);
	rMatrix.values[2][0] = axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle);
	rMatrix.values[2][1] = axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle);
	rMatrix.values[2][2] = axis.z * axis.z * (1 - cos(angle)) + cos(angle);

	matrice rmPoint(3, 1);

	rmPoint = rMatrix * mPoint;

	rPoint.x = rmPoint.values[0][0];
	rPoint.y = rmPoint.values[1][0];
	rPoint.z = rmPoint.values[2][0];

	return rPoint;

}
