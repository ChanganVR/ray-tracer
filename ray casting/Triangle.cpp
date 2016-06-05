#include "Triangle.h"
#include "plane.h"
#include "gl/glut.h"

Triangle::Triangle(Vec3f & a, Vec3f & b, Vec3f & c, Material * m)
	:Object3D(m), v0_(a), v1_(b), v2_(c)
{
	Vec3f::Cross3(normal_, (v1_ - v0_), (v2_ - v0_));
	normal_.Normalize();
	e01_ = v1_ - v0_;
	e12_ = v2_ - v1_;
	e20_ = v0_ - v2_;
}

bool Triangle::intersect(const Ray & r, Hit & h, float tmin)
{
	Plane plane(v0_, normal_, material_);
	Hit plane_hit;
	bool is_intersect = plane.intersect(r, plane_hit, tmin);
	if (is_intersect)
	{
		Vec3f point = plane_hit.getIntersectionPoint();
		Vec3f pe;
		Vec3f::Cross3(pe, point - v0_, e01_);
		if (normal_.Dot3(pe) > 0)
			return false;
		Vec3f::Cross3(pe, point - v1_, e12_);
		if (normal_.Dot3(pe) > 0)
			return false;
		Vec3f::Cross3(pe, point - v2_, e20_);
		if (normal_.Dot3(pe) > 0)
			return false;
		float t = plane_hit.getT();
		if (t< tmin || t >h.getT())
			return false;
		else
		{
			h.set(t, material_, normal_, r);
		}
	}
	else
		return false;
}

void Triangle::paint()
{
	material_->glSetMaterial();
	glBegin(GL_TRIANGLES);
	glNormal3f(normal_.x(), normal_.y(), normal_.z());
	glVertex3f(v0_.x(), v0_.y(), v0_.z());
	glVertex3f(v1_.x(), v1_.y(), v1_.z());
	glVertex3f(v2_.x(), v2_.y(), v2_.z());
	glEnd();
}
