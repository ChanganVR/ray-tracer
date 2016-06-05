#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vectors.h"

class Camera {
public:
	virtual Ray GenerateRay(Vec2f point) = 0;
	virtual float GetTMin()const = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;
};

#endif