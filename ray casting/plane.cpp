#include "plane.h"
#include "gl/glut.h"

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

void Plane::paint()
{
	Vec3f n1, n2;
	if (normal_ == Vec3f(1, 0, 0))
	{
		n1 = Vec3f(0, 1, 0);
		n2 = Vec3f(0, 0, 1);
	}
	else
	{
		Vec3f::Cross3(n1, Vec3f(1, 0, 0), normal_);
		Vec3f::Cross3(n2, normal_, n1);
		n1.Normalize();
		n2.Normalize();
	}
	float size = 1000000;
	Vec3f center(normal_*d_);
	Vec3f v1 = center + n1*size + n2*size;
	Vec3f v2 = center + n1*size - n2*size;
	Vec3f v3 = center - n1*size + n2*size;
	Vec3f v4 = center - n1 *size - n2*size;
	material_->glSetMaterial();
	glBegin(GL_QUADS);
	glNormal3f(normal_.x(), normal_.y(), normal_.z());
	glVertex3f(v1.x(), v1.y(), v1.z());
	glVertex3f(v2.x(), v2.y(), v2.z());
	glVertex3f(v4.x(), v4.y(), v4.z());
	glVertex3f(v3.x(), v3.y(), v3.z());
	glEnd();
}
