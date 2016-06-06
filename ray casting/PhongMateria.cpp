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

#include "PhongMaterial.h"
#include "gl/glut.h"

Vec3f PhongMaterial::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
	Vec3f object_color;
	Vec3f object_normal = hit.getNormal();

	//view point is in the backside of the object
	if (object_normal.Dot3(ray.getDirection()) > 0)
	{
		object_color.Set(0, 0, 0);
	}

	Vec3f view_color;
	//diffuse reflection
	float clamped = object_normal.Dot3(dirToLight);
	if (clamped < 0)
		clamped = 0;
	Vec3f diffuse_color;
	Vec3f::Mult(diffuse_color, lightColor, hit.getMaterial()->getDiffuseColor());
	diffuse_color *= clamped;

	//specular reflection
	Vec3f halfway_vector = dirToLight - ray.getDirection();
	halfway_vector.Normalize();
	clamped = object_normal.Dot3(halfway_vector);
	if (clamped < 0)
		clamped = 0;
	else
		clamped = pow(clamped, exponent_);
	Vec3f specular_color;
	Vec3f::Mult(specular_color, lightColor, specularColor_);
	specular_color *= clamped;

	object_color += diffuse_color + specular_color;
	return object_color;
}


// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

void PhongMaterial::glSetMaterial(void) const {

	GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat specular[4] = {
		getSpecularColor().r(),
		getSpecularColor().g(),
		getSpecularColor().b(),
		1.0 };
	GLfloat diffuse[4] = {
		getDiffuseColor().r(),
		getDiffuseColor().g(),
		getDiffuseColor().b(),
		1.0 };

	// NOTE: GL uses the Blinn Torrance version of Phong...      
	float glexponent = exponent_;
	if (glexponent < 0) glexponent = 0;
	if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX 

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

	// OPTIONAL: 3 pass rendering to fix the specular highlight
	// artifact for small specular exponents (wide specular lobe)

	if (SPECULAR_FIX_WHICH_PASS == 0) {
		// First pass, draw only the specular highlights
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

	}
	else if (SPECULAR_FIX_WHICH_PASS == 1) {
		// Second pass, compute normal dot light 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}
	else {
		// Third pass, add ambient & diffuse terms
		assert(SPECULAR_FIX_WHICH_PASS == 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}

#endif
}