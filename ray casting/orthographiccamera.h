#pragma once

#include "vectors.h"
#include "camera.h"
#include "windows.h"
#include "gl/GL.h"
#include "gl/GLU.h"
#include "gl/glut.h"

class OrthographicCamera :Camera {
private:
	Vec3f center_;
	Vec3f up_;
	Vec3f horizontal_;
	Vec3f direction_;
	float size_;
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size) 
		:center_(center), up_(up), direction_(direction), size_(size) 
	{
		Vec3f::Cross3(horizontal_, direction_, up_);
		Vec3f::Cross3(up_, horizontal_, direction_);
		direction_.Normalize();
		horizontal_.Normalize();
		up_.Normalize();
	}
	virtual Ray GenerateRay(Vec2f point)
	{
		Vec3f origin = center_ + (point.x() - 0.5) * size_ * horizontal_ + (point.y() - 0.5) * size_ * up_;
		return Ray(origin, direction_);
	}
	virtual float GetTMin()const
	{
		return -INT_MAX;//assume ray start from negative infinity
	}
	void glInit(int w, int h)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w > h) 
			glOrtho(-size_ / 2.0, size_ / 2.0, -size_*(float)h / (float)w / 2.0, size_*(float)h / (float)w / 2.0, 0.5, 40.0);
		else 
			glOrtho(-size_*(float)w / (float)h / 2.0, size_*(float)w / (float)h / 2.0, -size_ / 2.0, size_ / 2.0, 0.5, 40.0);
	}	
	void glPlaceCamera(void) 
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