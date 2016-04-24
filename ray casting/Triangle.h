#pragma once

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vectors.h"
#include "object3d.h"	

class Triangle :public Object3D {
private:
	Vec3f v0_;
	Vec3f v1_;
	Vec3f v2_;
	Vec3f e01_;
	Vec3f e12_;
	Vec3f e20_;
	Vec3f normal_;
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
	bool intersect(const Ray &r, Hit &h, float tmin);
};

#endif 