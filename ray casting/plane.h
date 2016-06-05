#pragma once

#ifndef PLANE_H
#define PLANE_H

#include "vectors.h"
#include "object3d.h"	

class Plane :public Object3D{
private:
	Vec3f normal_;
	float d_;
public:
	Plane(Vec3f &normal, float d, Material *m);
	Plane(const Vec3f &v0, const Vec3f &normal, Material *m);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint();
};

#endif 