#pragma once

#include "vectors.h"
#include "camera.h"
#include "windows.h"
#include "gl/gl.h"
#include "gl/glu.h"


class PerspectiveCamera :Camera {
private:
	Vec3f center_;
	Vec3f direction_;
	Vec3f horizontal_;
	Vec3f up_;
	float angle_;
public:
	PerspectiveCamera(Vec3f center, Vec3f &direction, Vec3f &up, float angle) :
		center_(center), direction_(direction), up_(up), angle_(angle)
	{
		Vec3f::Cross3(horizontal_, direction_, up_);
		Vec3f::Cross3(up_, horizontal_, direction_);
		direction_.Normalize();
		horizontal_.Normalize();
		up_.Normalize();
	}
	Ray GenerateRay(Vec2f point) 
	{
		float angle = sin(angle_ / 2) / sqrt(cos(angle_)) * 2;
		Vec3f direction(direction_);
		Vec3f biased_up = tan((point.y() - 0.5)*angle) * up_;
		Vec3f biased_horizontal = tan((point.x() - 0.5)*angle) * horizontal_;
		direction += biased_horizontal + biased_up;
		direction.Normalize();
		return Ray(center_, direction);
	}
	float GetTMin()const 
	{
		return 0;
	}

	void PerspectiveCamera::glInit(int w, int h) 
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(angle_*180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
	}
	void PerspectiveCamera::glPlaceCamera(void) 
	{
		gluLookAt(center_.x(), center_.y(), center_.z(), center_.x() + direction_.x(), 
			center_.y() + direction_.y(), center_.z() + direction_.z(), up_.x(), up_.y(), up_.z());
	}
	void dollyCamera(float dist)
	{
		//cout << "dolly ";
		center_ += direction_*dist;
	}
	void truckCamera(float dx, float dy)
	{
		//cout << "truck ";
		center_ += horizontal_*dx + up_*dy;
		//Vec3f horizontal;
		//Vec3f::Cross3(horizontal, direction_, up_); 
		//horizontal.Normalize(); 
		//Vec3f screenUp; 
		//Vec3f::Cross3(screenUp, horizontal, direction_); 
		//center_ += horizontal*dx + screenUp*dy;
	}
	void rotateCamera(float rx, float ry)
	{
		//cout << "rotate ";
		Matrix rotMat = Matrix::MakeAxisRotation(up_, rx);
		rotMat *= Matrix::MakeAxisRotation(horizontal_, ry);
		rotMat.Transform(center_);
		rotMat.Transform(direction_);
		rotMat.Transform(horizontal_);
		rotMat.Transform(up_);
		//Vec3f horizontal; 
		//Vec3f::Cross3(horizontal, direction_, up_); 
		//horizontal.Normalize();
		//float tiltAngle = acos(up_.Dot3(direction_));
		//if (tiltAngle - ry > 3.13)
		//	ry = tiltAngle - 3.13;
		//else if (tiltAngle - ry < 0.01)
		//{
		//	ry = tiltAngle - 0.01; 
		//	Matrix rotMat = Matrix::MakeAxisRotation(up_, rx);
		//	rotMat *= Matrix::MakeAxisRotation(horizontal, ry); 
		//	rotMat.Transform(center_);
		//	rotMat.TransformDirection(direction_);
		//}
	}
};