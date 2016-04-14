#pragma once

#include "ray.h"
#include "vectors.h"

class Camera {
public:
	virtual Ray GenerateRay(Vec2f point) = 0;
	virtual float GetTMin()const = 0;
};