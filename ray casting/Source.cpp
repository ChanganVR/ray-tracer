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

int main(int argc, char *argv[])
{
	char *input_file = "scene1_06.txt";
	int width = 200;
	int height = 200;
	char *output_file = "output1_06.tga";
	float depth_min = 3;
	float depth_max = 7;
	char *depth_file = "depth1_06.tga";

	// sample command line:
	// raytracer -input scene1_1.txt -size 200 200 -output output1_01.tga -depth 9 10 depth1_1.tga

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
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	SceneParser parser(input_file);
	Camera *camera = parser.getCamera();
	Vec3f background_color = parser.getBackgroundColor();
	int num_materials = parser.getNumMaterials();
	Material **materials = (Material**) new Material* [num_materials];
	for (int i = 0; i < num_materials; i++)
	{
		materials[i] = parser.getMaterial(i);
	}
	Group *obj_group = parser.getGroup();

	int tmin = camera->GetTMin();
	Image image(width, height);
	Image image2(width, height);
	image.SetAllPixels(background_color);
	image2.SetAllPixels(Vec3f(0,0,0));
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
				image.SetPixel(i, j, hit.getMaterial()->getDiffuseColor());
				float t = hit.getT();
				if (t<depth_max && t>depth_min)
				{
					float gray_value = 1 - (t - depth_min) / (depth_max - depth_min);
					Vec3f color(gray_value, gray_value, gray_value);
					image2.SetPixel(i, j, color);
				}
			}
		}
	}
	image.SaveTGA(output_file);
	image2.SaveTGA(depth_file);
}