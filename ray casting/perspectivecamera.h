#pragma once

#include "vectors.h"
#include "camera.h"

class PerspectiveCamera :Camera {
private:
	Vec3f center_;
	Vec3f direction_;
	Vec3f horizontal_;
	Vec3f up_;
	float angle_;
public:
	PerspectiveCamera(Vec3f center, Vec3f &direction, Vec3f &up, float angle) :
		center_(center), direction_(direction), up_(up), angle_(angle)
	{
		Vec3f::Cross3(horizontal_, direction_, up_);
		Vec3f::Cross3(up_, horizontal_, direction_);
		direction_.Normalize();
		horizontal_.Normalize();
		up_.Normalize();
	}
	Ray GenerateRay(Vec2f point) 
	{
		float angle = sin(angle_ / 2) / sqrt(cos(angle_)) * 2;
		Vec3f direction(direction_);
		Vec3f biased_up = tan((point.y() - 0.5)*angle) * up_;
		Vec3f biased_horizontal = tan((point.x() - 0.5)*angle) * horizontal_;
		direction += biased_horizontal + biased_up;
		direction.Normalize();
		return Ray(center_, direction);
	}
	float GetTMin()const 
	{
		return 0;
	}
};