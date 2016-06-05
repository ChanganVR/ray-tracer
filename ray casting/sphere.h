#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "vectors.h"
#include "object3d.h"

inline float square(float a) { return a*a; }

class Sphere :public Object3D {
private:
	Vec3f center_;
	float radius_;
public:
	Sphere(Vec3f center, float radius, Material *material)
		:Object3D(material),center_(center),radius_(radius){}
	~Sphere() {}
	//void setAngle(float theta, float phi) { theta = theta; phi = phi; }
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint();
};

#endif