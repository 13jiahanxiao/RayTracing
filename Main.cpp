#include <iostream>
#include<fstream>
#include"vector.h"
#include"ray.h"
#include"hittable.h"
#include"camera.h"
#include"material.h"
#include"sphere.h"


Hittable* RandomScene() {
	int n = 20;
	Hittable** list = new Hittable * [n + 1];
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -3; a <3; a+=2) {
		for (int b = -3; b < 3; b+=2) {
			float chooseMat = RandomDouble();
			Vec3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());
			if ((center - Vec3(4, 0.2, 0)).Length() > 0.9) {
				if (chooseMat < 0.8) {  // diffuse
					list[i++] = new Sphere(center, 0.2,
						new Lambertian(Vec3(RandomDouble() * RandomDouble(),
							RandomDouble() * RandomDouble(),
							RandomDouble() * RandomDouble())
						)
					);
				}
				else if (chooseMat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vec3(0.5 * (1 + RandomDouble()),
							0.5 * (1 + RandomDouble()),
							0.5 * (1 + RandomDouble())),
							0.5 * RandomDouble()));
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
	return new HittableList(list, i);
}

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
	int nx = 800;
	int ny = 600;
	int ns = 100;
	Vec3 lookfrom(13, 2, 3);
	Vec3 lookat(0, 0, 0);
	float distToFocus = 10;
	float aperture = 0.1;

	Camera cam(lookfrom, lookat, Vec3(0, 1, 0), 20,	float(nx) / float(ny), aperture, distToFocus);

	Hittable* world = RandomScene();
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
	Outfile.close();
	return 0;
}