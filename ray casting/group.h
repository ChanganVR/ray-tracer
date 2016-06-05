#pragma once

#include <vector>
#include <assert.h>
#include "object3d.h"

class Group : Object3D{
private:
	int obj_num_;
	vector<Object3D*> pobj_;
public:
	Group(int n) :Object3D(NULL){ obj_num_ = n; }
	void addObject(int index, Object3D *obj) { pobj_.push_back(obj); };
	virtual void paint() 
	{
		for (int i = 0; i < obj_num_; i++)
		{
			pobj_[i]->paint();
		}
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		bool is_intersect = false;
		for (int i = 0; i < obj_num_; i++)
		{
			/* notice: is_intersect = is_intersect || pobj->intersect is wrong, 
			because once is_intersect is true, pobj->intersect will not be executed*/
			is_intersect = pobj_[i]->intersect(r, h, tmin) || is_intersect;
		}
		return is_intersect;
	}
};