#pragma once

#include "vectors.h"
#include "camera.h"

class OrthographicCamera :Camera {
private:
	Vec3f center_;
	Vec3f up_;
	Vec3f horizontal_;
	Vec3f direction_;
	float size_;
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size) 
		:center_(center), up_(up), direction_(direction), size_(size) 
	{
		Vec3f::Cross3(horizontal_, direction_, up_);
		Vec3f::Cross3(up_, horizontal_, direction_);
		direction_.Normalize();
		horizontal_.Normalize();
		up_.Normalize();
	}
	virtual Ray GenerateRay(Vec2f point)
	{
		Vec3f origin = center_ + (point.x() - 0.5) * size_ * horizontal_ + (point.y() - 0.5) * size_ * up_;
		return Ray(origin, direction_);
	}
	virtual float GetTMin()const 
	{
		return -INT_MAX;//assume ray start from negative infinity
	}
};