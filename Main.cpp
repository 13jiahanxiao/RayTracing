#include <iostream>
#include<fstream>
#include"vector.h"
#include"ray.h"
#include"hittable.h"
#include"camera.h"
#include"material.h"
#include"sphere.h"


Vec3 Color(const Ray& r, Hittable* world, int depth)
{
	HitRecord temp;
	if (world->Hit(r, 0.001, FLT_MAX, temp)) {
		Vec3 attenuation;
		Ray scatter;
		if (depth < 50 && temp.material->Scatter(r, temp, attenuation, scatter))
		{
			return attenuation * Color(scatter, world, depth + 1);
		}
		else
		{
			return Vec3(0, 0, 0);
		}
	}
	else
	{
		Vec3 unit_direction = UnitVector(r.Direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
	}
}

int main()
{
	int nx = 1280;
	int ny = 720;
	int ns = 100;
	Camera cam;

	Hittable* list[4];
	list[0] = new Sphere(Vec3(0, 0, -3), 2.0f, new Lambertian(Vec3(0.8, 0.3, 0.3)));
	list[1] = new Sphere(Vec3(0, -100, -3), 98, new Lambertian(Vec3(0.8, 0.8, 0)));
	list[2] = new Sphere(Vec3(4, 0, -3), 2.0f, new Metal(Vec3(0.8, 0.6, 0.2)));
	list[3] = new Sphere(Vec3(-4, 0, -3), 2.0f, new Metal(Vec3(0.8, 0.8, 0.8)));
	Hittable* world = new HittableList(list, 4);
	std::ofstream Outfile("MyTest.txt", std::ios::out);
	Outfile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
		for (int i = 0; i < nx; i++) {
			Vec3 color(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = (float)(i + RandomDouble()) / (float)nx;
				float v = (float)(j + RandomDouble()) / (float)ny;
				Ray r = cam.GetRay(u, v);
				color += Color(r, world, 0);
			}
			color /= float(ns);
			color = Vec3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));
			int ir = (int)255.99 * color[0];
			int ig = (int)255.99 * color[1];
			int ib = (int)255.99 * color[2];
			Outfile << ir << " " << ig << " " << ib << "\n";
		}
}