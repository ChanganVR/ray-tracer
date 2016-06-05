#include "sphere.h"
#include "gl/glut.h"

inline float Cos(float degree)
{
	return cos(3.1415*degree / 180);
}

inline float Sin(float degree)
{
	return sin(3.1415*degree / 180);
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
	Vec3f origin = r.getOrigin();
	Vec3f direction = r.getDirection();
	direction.Normalize();

	Vec3f origin_to_center = center_ - origin;
	float val_origin_to_center = origin_to_center.Dot3(origin_to_center);
	bool is_inside = false;
	if (val_origin_to_center < square(radius_))//origin point is inside the sphere
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
			Vec3f normal = r.pointAtParameter(t) - center_;
			normal.Normalize();
			h.set(t, material_, normal, r);
			return true;
		}
	}
}

void Sphere::paint()
{
	extern bool is_tessellation;
	extern bool is_gouraud;
	extern float theta_time;
	extern float phi_time;
	assert(is_tessellation);

	float theta_increment = 360 / theta_time;
	float phi_increment = 180 / phi_time;
	for (float phi = -90; phi <= 90.1; phi += phi_increment)
		for (float theta = 0; theta <= 360.1; theta += theta_increment)
		{
			//float phi = -90;
			//float theta = 0;
			Vec3f n1(Cos(phi) * Cos(theta), Cos(phi) * Sin(theta), Sin(phi));
			Vec3f v1 = center_ + n1*radius_;
			Vec3f n2(Cos(phi) * Cos(theta + theta_increment), Cos(phi) * Sin(theta + theta_increment), Sin(phi));
			Vec3f v2 = center_ + n2*radius_;
			Vec3f n3(Cos(phi + phi_increment) * Cos(theta), Cos(phi + phi_increment) * Sin(theta), Sin(phi + phi_increment));
			Vec3f v3 = center_ + n3*radius_;
			Vec3f n4(Cos(phi + phi_increment) * Cos(theta + theta_increment), Cos(phi + phi_increment) * Sin(theta + theta_increment), Sin(phi + phi_increment));
			Vec3f v4 = center_ + n4*radius_;
			Vec3f normal(Cos(phi + phi_increment /2) * Cos(theta + theta_increment / 2), Cos(phi + phi_increment /2) * Sin(theta + theta_increment / 2), Sin(phi + phi_increment / 2));
			normal.Normalize();

			material_->glSetMaterial();
			glBegin(GL_QUADS);// the vertices should follow clockwise order
			if (is_gouraud)
			{
				//gouraud gives every vertex a normal and opengl will do the interpolation to make surface more flexible
				glNormal3f(n1.x(), n1.y(), n1.z());
				glVertex3f(v1.x(), v1.y(), v1.z());
				glNormal3f(n2.x(), n2.y(), n3.z());
				glVertex3f(v2.x(), v2.y(), v2.z());
				glNormal3f(n4.x(), n4.y(), n4.z());
				glVertex3f(v4.x(), v4.y(), v4.z());
				glNormal3f(n3.x(), n3.y(), n3.z());
				glVertex3f(v3.x(), v3.y(), v3.z());
			}
			else
			{
				//flat shading make every piece of surface flat
				glNormal3f(normal.x(), normal.y(), normal.z());
				glVertex3f(v1.x(), v1.y(), v1.z());
				glVertex3f(v2.x(), v2.y(), v2.z());
				glVertex3f(v4.x(), v4.y(), v4.z());
				glVertex3f(v3.x(), v3.y(), v3.z());
			}
			glEnd();
		}
}
