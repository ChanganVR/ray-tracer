#include "transform.h"
#include "gl/glut.h"

Transform::Transform(Matrix & m, Object3D * o)
	:rev_matrix_(m), object_(o), Object3D(nullptr)
{
	assert(rev_matrix_.Inverse());
}

bool Transform::intersect(const Ray & r, Hit & h, float tmin)
{
	Vec3f origin(r.getOrigin());
	Vec3f direction(r.getDirection());
	direction.Normalize();
	rev_matrix_.Transform(origin);
	rev_matrix_.TransformDirection(direction);
	direction.Normalize();
	Ray ray(origin, direction);
	/*transforming makes t inconformity between object space and world space, 
	check t and rescale after intersecting*/
	Hit hit;
	bool is_intersect = object_->intersect(ray, hit, tmin);
	float diagonal[3] = { rev_matrix_.Get(0,0),rev_matrix_.Get(1,1),rev_matrix_.Get(2,2) };
	if (diagonal[0] == diagonal[1] && diagonal[1] == diagonal[2])
	{
		float t = hit.getT() / diagonal[0];
		if (t < h.getT())
			h.set(t, hit.getMaterial(), hit.getNormal(), ray);
	}
	if (is_intersect)
	{
		Vec3f trans_normal(h.getNormal());
		Matrix transpose_matrix(rev_matrix_);
		transpose_matrix.Transpose();
		transpose_matrix.Transform(trans_normal);
		trans_normal.Normalize();
		h.set(h.getT(), h.getMaterial(), trans_normal, r);
	}
	return is_intersect;
}

void Transform::paint()
{
	//TODO: Matrix trans_matrix = Matrix(rev_matrix_);
	glPushMatrix();
	GLfloat *glMatrix = rev_matrix_.glGet();
	glMultMatrixf(glMatrix);
	object_->paint();
	delete[] glMatrix;
	glPopMatrix();
}
