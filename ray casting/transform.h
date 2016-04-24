#pragma once

#ifndef TRANSFORM_H
#define	TRANSFORM_H

#include "vectors.h"
#include "matrix.h"
#include "object3d.h"	

class Transform :public Object3D {
private:
	Matrix rev_matrix_;
	Object3D *object_;
public:
	Transform(Matrix &m, Object3D *o);
	bool intersect(const Ray &r, Hit &h, float tmin);
};

#endif 