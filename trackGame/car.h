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
	Vector3 targetNormal = Vector3Zero();
	int bumpFrames = 0;
	Vector3 angleMovement = Vector3Zero();//for the bump
	double speed = 0, displaySpeed = 0;//first is in u/f(units/frame), second is in km/h
	short int gear = 1;
	float rpm = 0;
	int accelCooldown = 0;//cooldown for accelerating after gear change
	bool automatic = false;
	float rpmAccel[9] = {-2.5f, 6.0f, 4.0f, 3.0f, 2.0f, 1.0f, 0.4f, 0.2f, 0.02f};//R 1 2 3 4 5 6 7 >3000rpm
	float minSpeed[8] = { 0.0f, 0.0f, 150.0f, 300.0f, 450.0f, 600.0f, 750.0f, 900.0f };//R 1 2 3 4 5 6 7
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
	
	/*void updateCollisionSpheresPos()
	{
		for (auto& s : collisionSpheres)
		{
			//yzx
			s.localCenter = Vector3RotateByAxisAngle(s.localCenter, up, angle.y - oldAngle.y);
			s.localCenter = Vector3RotateByAxisAngle(s.localCenter, right, angle.z - oldAngle.z);
			s.localCenter = Vector3RotateByAxisAngle(s.localCenter, forward, angle.x - oldAngle.x);

			s.center = Vector3Add(position, s.localCenter);
		}
	}*/

	void updateCollisionSpheresPos()
	{
		for (auto& s : collisionSpheres)
		{
			Vector3 offset = Vector3Zero();

			offset = Vector3Add(offset, Vector3Scale(forward, s.localCenter.x));
			offset = Vector3Add(offset, Vector3Scale(up, s.localCenter.y));
			offset = Vector3Add(offset, Vector3Scale(right, s.localCenter.z));

			s.center = Vector3Add(position, offset);
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
		bool collided = false;
		float dist = 10000.0f, pushDistT;
		Vector3 normalT = up;
		Sphere sp = collisionSpheres[sphereId];
		for (auto b : track.blocks)
		{
			if (Vector3DistanceSqr(sp.center, b.position) <= (b.size + 0.5) * (b.size + 0.5))
			{
				float distToBlock = Vector3DistanceSqr(position, b.position);

				Matrix transform = MatrixTranslate(b.position.x, b.position.y, b.position.z);

				bool hit = CheckCollisionSphereMesh(sp.center, sp.radius, position, b.blockModel.meshes[0], transform, normalT, pushDistT);

				if (hit)
				{
					collided = true;
					if (distToBlock < dist)
					{
						dist = distToBlock;
						normal = normalT;
						pushDist = pushDistT;
					}
					//std::cout << "wheel " << sphereId << " hit with block at : " << b.position.x << ' ' << b.position.y << ' ' << b.position.z << '\n';
					//std::cout << "normal: " << normal.x << ' ' << normal.y << ' ' << normal.z << '\n';
				} 
			}
		}
		return collided;
	}

	void updateCollision(trackData track)
	{
		Vector3 normal = up, averageNormal = Vector3Zero();
		float spheresTouch = 0;
		float maxPushDist = 0;
		//improve the block loading and storing(use a 3d vector and simplify the block class if possible)

		for (int i = 0; i < collisionSpheres.size(); i++)
		{
			float pushDist;
			if (collisionCheck(track, i, normal, pushDist))
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
		std::cout << "pushDist: " << maxPushDist << "\n\n";

		position = Vector3Add(position, Vector3Scale(averageNormal, maxPushDist));

		//moving the car so up becomes averageNormal

		if (bumpFrames == 0 && Vector3Angle(up, averageNormal) > 0.01f)
		{
			bumpFrames = 20;//16 to match, 4 to overshoot a little
			targetNormal = averageNormal;

			//calculate the angles i need to apply to the car

			if (true)//hitting a floor
			{
				Vector3 forwardProjection = ProjectVectorOntoPlane(targetNormal, right);

				angleMovement.z = (SignedAngle(up, forwardProjection, right)) / 16.0f;

				angleMovement.z *= -1;

				Vector3 rightProjection = ProjectVectorOntoPlane(targetNormal, forward);

				angleMovement.x = (SignedAngle(up, rightProjection, forward)) / 16.0f;

				angleMovement.x *= -1;
				
				angleMovement.y = 0.0f;
			}
			else if (false)//hitting a wall(add distinction between right forward and their opposites)
			{

			}
			else if (false)//hitting a roof
			{

			}

		}

		if (bumpFrames > 0)
		{
			bumpFrames--;
			angle = Vector3Subtract(angle, angleMovement);
		}
		
	}

	void gearUpdate()
	{
		if (IsKeyPressed(KEY_Z))//change transmission
		{
			automatic = !automatic;
		}

		if (automatic)
		{
			if (rpm >= 3000.0f && gear < 7)//gear up(automatic) 
			{
				gear++;
				rpm -= 2999.999f;
				accelCooldown = 35;
			}
			if (rpm <= 0.0f && gear > 1)//gear down(automatic)
			{
				gear--;
				rpm += 3000.0f;
				accelCooldown = 35;
			}
		}
		else
		{
			if (IsKeyPressed(KEY_P) && rpm >= 2950.0f && gear < 7)//gear up(manual) 
			{
				gear++;
				rpm -= 3000.0f;
				accelCooldown = 35;
			}
			if (IsKeyPressed(KEY_L) && gear > 1)//gear down(manual)
			{
				gear--;
				rpm += 3000.0f;
				accelCooldown = 35;
			}
		}

		if (gear == 0 && rpm >= 0.0f)//going from reverse to first gear
			gear = 1;

		if (IsKeyDown(KEY_UP))//if accelerating
		{
			if (rpm > 3000.0f)//if rpm too high make the acceleration bad(to prevent staying in low gear for the better acceleration)
				if (accelCooldown == 0)
					rpm += rpmAccel[8];
				else
					accelCooldown--;
			else//if rpm normal
			{
				if (gear == 0)//if pressing forward in reverse gear, apply a quick accel to get fast in 1st gear
					rpm += rpmAccel[1] * 3;
				else if (accelCooldown == 0)//if in 1st - 7th gear, and no accel cooldown
					rpm += rpmAccel[gear];//accelerate depending on the gear(the higher the worst the acceleration)
				else if (accelCooldown > 0)
					accelCooldown--;
			}
		}
		else if (rpm > 0.0f)//if not pressing up, to decelerate
		{
			if (rpm > 3000.0f)//if rpm too high decelerate fast(3* the accel in 1st gear)
				rpm -= rpmAccel[1] * 3;
			else//if rpm 0-3000 decelerate slightly quicker than accelerating
				rpm -= rpmAccel[gear] + 1.0f;
		}
		else if (rpm < 0.0f && !IsKeyDown(KEY_DOWN) && gear > 0)//if rpm is negative try to get it to 0, when not braking(get the car to the minimum speed of the gear)(if gear 0, that means the car will accelerate backwards when not holding down, so we avoid it)
		{
			if (rpm > -rpmAccel[gear])//if rpm close to 0, just make it 0
				rpm = 0.0f;
			else//else apply the acceleration of the current gear(faster to gear down and accelerate then staying in the higher gear)
				rpm += rpmAccel[gear];
		}

		if (IsKeyDown(KEY_DOWN))//if braking/accelerating backwards
		{
			if (gear >= 1 && rpm <= (gear - 1) * (-3000.0f))//if in gear higher than 1 with low enough rpm to have negative speed switch to reverse and make the rpm higher
			{
				rpm += (gear - 1) * 3000.0f;
				gear = 0;
			}
			if (gear == 0 && rpm > -3000.0f)//if in reverse and slower than 150 backwards speed(-3000 rpm) accelerate backwards
				rpm += rpmAccel[gear];
			else if (gear != 0)//if not in reverse and braking, decelerate faster than the backwards acceleration
				rpm += rpmAccel[0] * 2.5f;
		}
		else if (gear == 0)//if not pressing down and in gear 0
		{
			if (rpm > rpmAccel[gear])//if close to 0 rpm make it 0
				rpm = 0.0f;
			else//else accelerate to 0 rpm
				rpm -= rpmAccel[gear];
		}

		if (rpm >= (2000.0f + (7 - gear) * 3000.0f))//limit the car to 1000 speed (2000 rpm in gear 7)
		{
			rpm = (2000.0f + (7 - gear) * 3000.0f);
		}
	}

	void updatePhysics(Camera& camera, trackData track)
	{
		gearUpdate();
		
		displaySpeed = minSpeed[gear] + 150.0f * (rpm / 3000.0f);//calculate the display speed in km/h

		speed = displaySpeed / 1440.0f;//calculate the speed applied to the car

		oldAngle = angle;

		if (speed == 0)
			speed = 1e-10;

		if (speed != 1e-10)
		{
			angle.y += speed / abs(speed) * DEG2RAD * 0.5 * (-IsKeyDown(KEY_RIGHT) + IsKeyDown(KEY_LEFT));
		}
		else
		{
			angle.x -= speed / abs(speed) * DEG2RAD * 0.5 * (-IsKeyDown(KEY_H) + IsKeyDown(KEY_F));
			angle.z += speed / abs(speed) * DEG2RAD * 0.5 * (-IsKeyDown(KEY_T) + IsKeyDown(KEY_G));
		}

		position = Vector3Add(position, Vector3Scale(forward, speed));

		//position = Vector3Add(position, { 0.0f, -0.1f, 0.0f });//hard coded gravity for now, will update when organizing the code

		if (IsKeyPressed(KEY_SPACE))
			position = Vector3Add(position, Vector3{ 0.0f, 2.3f, 0.0f });
		
		updateCollision(track);

		newCarAxisUpdate();

		updateCollisionSpheresPos();

		camera.target = position;
		camera.position = Vector3Add(position, Vector3Add(Vector3Scale(forward, -10), Vector3Scale(up, 7.5)));
		camera.up = up;
	}
};

