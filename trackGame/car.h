#pragma once
#include <raylib.h>
#include <cmath>
#include <cstdlib>
#include <raymath.h>
#include "matrice.h"
#include "physics.h"
#include "trackData.h"
#include <vector>

struct Sphere {
	Vector3 center = Vector3Zero(), localCenter;
	float radius;
};

class car
{
public:
	Vector3 position = Vector3{ 0.0f, 3.0f, 0.0f};
	Vector3 angle = Vector3Zero(), oldAngle = Vector3Zero();
	Vector3 up = { 0.0f, 1.0f, 0.0f }, right = { 0.0f, 0.0f, 1.0f }, forward = {1.0f, 0.0f, 0.0f};
	double speed = 0;
	short int gear = 1;
	float rpm = 0;
	float deceleration = 0;
	float acceleration = 0;
	Model model;
	std::vector<Sphere> collisionSpheres;

	car() {
		Sphere temp;
		temp.radius = 0.19f;
		temp.localCenter = Vector3{ 0.8f, 0.1f, 0.4f };
		temp.center = Vector3{ 0.8f, 0.1f, 0.4f };
		collisionSpheres.push_back(temp);
		temp.localCenter = Vector3{ 0.8f, 0.1f, -0.4f };
		temp.center = Vector3{ 0.8f, 0.1f, -0.4f };
		collisionSpheres.push_back(temp);
		temp.localCenter = Vector3{ -0.8f, 0.1f, 0.4f };
		temp.center = Vector3{ -0.8f, 0.1f, 0.4f };
		collisionSpheres.push_back(temp);
		temp.localCenter = Vector3{ -0.8f, 0.1f, -0.4f };
		temp.center = Vector3{ -0.8f, 0.1f, -0.4f };
		collisionSpheres.push_back(temp);
	}
	
	void updateCollisionSpheresPos()
	{
		for (auto& s : collisionSpheres)
		{
			//yzx
			s.localCenter = Vector3RotateByAxisAngle(s.localCenter, up, angle.y - oldAngle.y);
			s.localCenter = Vector3RotateByAxisAngle(s.localCenter, right, angle.z - oldAngle.z);
			s.localCenter = Vector3RotateByAxisAngle(s.localCenter, forward, angle.x - oldAngle.x);

			s.center = Vector3Add(position, s.localCenter);
		}
	}

	void newCarAxisUpdate() 
	{
		//y
		forward = Vector3RotateByAxisAngle(forward, up, angle.y - oldAngle.y);
		right = Vector3RotateByAxisAngle(right, up, angle.y - oldAngle.y);
		//z
		forward = Vector3RotateByAxisAngle(forward, right, angle.z - oldAngle.z);
		up = Vector3RotateByAxisAngle(up, right, angle.z - oldAngle.z);
		//x
		up = Vector3RotateByAxisAngle(up, forward, angle.x - oldAngle.x);
		right = Vector3RotateByAxisAngle(right, forward, angle.x - oldAngle.x);
	}
	
	bool collisionCheck(trackData track, short int sphereId, Vector3& normal, float& pushDist)
	{
		Sphere sp = collisionSpheres[sphereId];
		for (auto b : track.blocks)
		{
			if (Vector3DistanceSqr(sp.center, b.position) <= (b.size + 0.5) * (b.size + 0.5))
			{

				Matrix transform = MatrixTranslate(b.position.x, b.position.y, b.position.z);

				bool hit = CheckCollisionSphereMesh(sp.center, sp.radius, b.blockModel.meshes[0], transform, normal, pushDist);

				if (hit)
				{
					return true;
					//std::cout << "wheel " << sphereId << " hit with block at : " << b.position.x << ' ' << b.position.y << ' ' << b.position.z << '\n';
					//std::cout << "normal: " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
				} 
			}
		}
		return false;
	}

	void updatePhysics(Camera& camera, trackData track)
	{
		speed = 0.1 * 1 * (IsKeyDown(KEY_UP) - IsKeyDown(KEY_DOWN));

		oldAngle = angle;

		if (speed == 0)
			speed = 1e-5;

		if (speed != 1e-5)
		{
			angle.x -= speed / abs(speed) * DEG2RAD * 0.5 * (-IsKeyDown(KEY_H) + IsKeyDown(KEY_F));
			angle.y += speed / abs(speed) * DEG2RAD * 0.5 * (-IsKeyDown(KEY_RIGHT) + IsKeyDown(KEY_LEFT));
			angle.z += speed / abs(speed) * DEG2RAD * 0.5 * (-IsKeyDown(KEY_T) + IsKeyDown(KEY_G));
		}

		newCarAxisUpdate();

		position = Vector3Add(position, Vector3Scale(forward, speed));

		position = Vector3Add(position, { 0.0f, -0.1f, 0.0f });//hard coded gravity for now, will update when organizing the code
		
		Vector3 normal = up, averageNormal = Vector3Zero();
		float spheresTouch = 0;
		float maxPushDist = 0;

		for(int i = 0; i<collisionSpheres.size(); i++)
		{
			float pushDist;
			if(collisionCheck(track, i, normal, pushDist))
			{
				averageNormal = Vector3Add(averageNormal, normal);
				spheresTouch++;
				if (pushDist > maxPushDist)
					maxPushDist = pushDist;
				std::cout << "normal: " << normal.x << ' ' << normal.y << ' ' << normal.z << " wheel: " << i << '\n';
			}
		}
		if (spheresTouch != 0)
			averageNormal = Vector3{ averageNormal.x / spheresTouch, averageNormal.y / spheresTouch , averageNormal.z / spheresTouch };
		else
			averageNormal = up;

		std::cout << "averageNormal: " << averageNormal.x << ' ' << averageNormal.y << ' ' << averageNormal.z << '\n';

		position = Vector3Add(position, Vector3Scale(averageNormal, maxPushDist));

		updateCollisionSpheresPos();

		camera.target = position;
		camera.position = Vector3Add(position, Vector3Add(Vector3Scale(forward, -10), Vector3Scale(up, 7.5)));
		camera.up = up;
	}
};

