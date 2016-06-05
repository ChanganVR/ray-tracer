#include <assert.h>

#include "scene_parser.h"
#include "image.h"
#include "vectors.h"
#include "object3d.h"
#include "sphere.h"
#include "hit.h"
#include "ray.h"
#include "camera.h"
#include "group.h"
#include "light.h"
#include "glCanvas.h"

bool is_tessellation = false;
bool is_gouraud = false;
float theta_time = 0;
float phi_time = 0;

Vec3f compute_diffuse_color(const Hit &h, const Ray &r, Vec3f ambient_light, int num_lights, Light **lights, bool is_shade_back)
{
	Vec3f object_color = h.getMaterial()->getDiffuseColor();
	Vec3f object_normal = h.getNormal();
	if (object_normal.Dot3(r.getDirection()) > 0)//view point is in the backside of the object
	{
		if (is_shade_back)
			object_normal *= -1;
		else
			object_color.Set(0, 0, 0);
	}
	Vec3f color;
	Vec3f::Mult(color, object_color, ambient_light);
	for (int i = 0; i < num_lights; i++)
	{
		Vec3f light_dir;
		Vec3f light_col;
		lights[i]->getIllumination(h.getIntersectionPoint(), light_dir, light_col);
		float clamped = object_normal.Dot3(light_dir);
		if (clamped < 0)
			clamped = 0;
		Vec3f single_light_color;
		Vec3f::Mult(single_light_color, light_col, object_color);
		single_light_color *= clamped;
		color += single_light_color;
	}
	return color;
}


int main(int argc, char *argv[])
{
	char *input_file = nullptr;
	int width = 200;
	int height = 200;
	char *output_file = nullptr;
	float depth_min = 3;
	float depth_max = 7;
	char *depth_file = nullptr;
	char *normal_file = nullptr;
	bool is_shade_back = false;
	bool is_gui = false;

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")) {
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back")) {
			is_shade_back = true;
		}
		else if (!strcmp(argv[i], "-gui")) {
			is_gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")) {
			i++; assert(i < argc);
			theta_time = atof(argv[i]);
			i++; assert(i < argc);
			phi_time = atof(argv[i]);
			is_tessellation = true;
		}
		else if (!strcmp(argv[i], "-gouraud")) {
			is_gouraud = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	/*read arguments from parser*/
	SceneParser parser(input_file);

	Camera *camera = parser.getCamera();
	int num_lights = parser.getNumLights();
	Light **lights = (Light**) new Light* [num_lights];
	for (int i = 0; i < num_lights; i++)
	{
		lights[i] = parser.getLight(i);
	}
	Vec3f ambient_light = parser.getAmbientLight();
	Vec3f background_color = parser.getBackgroundColor();
	int num_materials = parser.getNumMaterials();
	Material **materials = (Material**) new Material* [num_materials];
	for (int i = 0; i < num_materials; i++)
	{
		materials[i] = parser.getMaterial(i);
	}
	Group *obj_group = parser.getGroup();

	/*create image instances*/
	int tmin = camera->GetTMin();
	Image image(width, height);
	Image depth_image(width, height);
	Image normal_image(width, height);
	image.SetAllPixels(background_color);
	if(depth_file)
		depth_image.SetAllPixels(Vec3f(0,0,0));
	if(normal_file)
		normal_image.SetAllPixels(Vec3f(0,0,0));
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			/*pixel_point should vary from (0,0) to (1,1),
			which will be magnified size times in camera to be the view plane*/
			Vec2f pixel_point((float)i / width, (float)j / height);
			Hit hit;
			Ray ray = camera->GenerateRay(pixel_point);
			bool is_intersect = obj_group->intersect(ray, hit, tmin);
			if (is_intersect)
			{
				Vec3f diffuse_color = compute_diffuse_color(hit, ray, ambient_light, num_lights, lights, is_shade_back);
				image.SetPixel(i, j, diffuse_color);

				float t = hit.getT();
				if (depth_file && t<depth_max && t>depth_min)
				{
					float gray_value = 1 - (t - depth_min) / (depth_max - depth_min);
					Vec3f color(gray_value, gray_value, gray_value);
					depth_image.SetPixel(i, j, color);
				}
				if (normal_file)
				{
					Vec3f normal_color = hit.getNormal();
					normal_color.Set(abs(normal_color.x()), abs(normal_color.y()), abs(normal_color.z()));
					normal_image.SetPixel(i, j, normal_color);
				}
			}
		}
	}
	image.SaveTGA(output_file);
	if(depth_file)
		depth_image.SaveTGA(depth_file);
	if (normal_file)
		normal_image.SaveTGA(normal_file);

	/*real-time rendering*/
	GLCanvas canvas;
	if (is_gui)
	{
		canvas.initialize(&parser, nullptr);
	}
}