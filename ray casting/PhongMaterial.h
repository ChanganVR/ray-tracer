#ifndef _PHONGMATERIAL_H
#define _PHONGMATERIAL_H

#include "material.h"

// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight 
// artifact for small specular exponents (wide specular lobe)
// ====================================================================

// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "glCanvas.h"  

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

class PhongMaterial :public Material {
private:
	Vec3f specularColor_;
	float exponent_;
public:
	PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent) 
		:Material(diffuseColor), specularColor_(specularColor), exponent_(exponent) {}
	Vec3f getSpecularColor(void) const { return specularColor_; }
	virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const { return Vec3f(); };
	virtual void glSetMaterial(void) const;
};

#endif