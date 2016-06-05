#pragma once
#include "material.h"
#include "hit.h"
#include "ray.h"

class Object3D {
protected:
	Material* material_;
public:
	Object3D(Material *material):material_(material){}
	~Object3D(){}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() = 0;
};