#include "sphere.h"

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
	const Vec3f origin = r.getOrigin();
	const Vec3f direction = r.getDirection();

	Vec3f origin_to_center = center_ - origin;
	float val_origin_to_center = origin_to_center.Dot3(origin_to_center);
	bool is_inside = false;
	if (val_origin_to_center < sqrt(radius_))//origin point is inside the sphere
		is_inside = true;

	float t = INT_MAX;
	if (is_inside)
	{
		float val_direction_projection = origin_to_center.Dot3(direction) / sqrt(direction.Dot3(direction));
		float val_center_to_ray = val_origin_to_center - square(val_direction_projection);
		float rd = square(radius_) - val_center_to_ray;
		t = sqrt(rd) + val_direction_projection;
	}
	else
	{
		float val_direction_projection = origin_to_center.Dot3(direction) / sqrt(direction.Dot3(direction));
		if (val_direction_projection < 0)
			return false;
		float val_center_to_ray = val_origin_to_center - square(val_direction_projection);
		if (sqrt(val_center_to_ray) > radius_)
			return false;
		float rd = square(radius_) - val_center_to_ray;
		t = val_direction_projection - sqrt(rd);
	}
	if (t < tmin)
		return false;
	else
	{
		if (t > h.getT())
			return false;
		else
		{
			h.set(t, material_, r);
			return true;
		}
	}
}