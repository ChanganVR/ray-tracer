#include "plane.h"

Plane::Plane(Vec3f & normal, float d, Material * m)
	:Object3D(m), normal_(normal), d_(d)
{
	normal_.Normalize();//assume normal always point towards origin
}

Plane::Plane(const Vec3f & v0, const Vec3f & normal, Material * m)
	:Object3D(m), normal_(normal)
{
	d_=normal_.Dot3(v0);
}

bool Plane::intersect(const Ray & r, Hit & h, float tmin)
{

	bool is_parallel = !(normal_.Dot3(r.getDirection()));
	bool is_in_plane = !(abs(r.getOrigin().Dot3(normal_)) - abs(d_));
	if (is_parallel && !is_in_plane)
		return false;
	else if (is_in_plane)
	{
		h.set(0, material_, normal_, r);
		return true;
	}
	else
	{
		float t = (d_ - normal_.Dot3(r.getOrigin())) / normal_.Dot3(r.getDirection());
		if (t<tmin || t>h.getT())
			return false;
		else
		{
			h.set(t, material_, normal_, r);
			return true;
		}
	}
}
