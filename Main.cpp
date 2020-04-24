#include <iostream>
#include<fstream>
#include"vector.h"
#include"ray.h"
#include"hittable.h"
#include"camera.h"
#include"material.h"
#include"geometry.h"
#include <thread>

int width = 800;
int height = 600;
int rgbWidth = width * 3;
int samplingRate = 100;

int* pixelData;

Hittable* world;

Vector3 lookfrom(278, 278, -800);
Vector3 lookat(278, 278, 0);
float distToFocus = 10;
float aperture = 0.0;
float vFov = 40.0;

Camera camera(lookfrom, lookat, Vector3(0, 1, 0), vFov, float(width) / float(height), aperture, distToFocus, 0, 1);


Hittable* RandomScene() {
	int n = 10;
	Hittable** list = new Hittable * [n + 1];
	Texture* test = new TestTexture{ new ConstantTexture(Vector3(0.2,0.3,0.1)),new ConstantTexture(Vector3( 0.9,0.9,0.9)) };
	list[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(test));
	int i = 1;
	for (int a = -1; a <1; a++) {
		for (int b = -1; b < 1; b++) {
			float chooseMat = RandomDouble();
			Vector3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());
			if ((center - Vector3(4, 0.2, 0)).Length() > 0.9) {
				if (chooseMat < 0.8) {  // diffuse
					list[i++] = new Sphere(center, 0.2,new Lambertian(new ConstantTexture( Vector3(RandomDouble() * RandomDouble(),RandomDouble() * RandomDouble(),RandomDouble() * RandomDouble()))));
				}
				else if (chooseMat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,	new Metal(Vector3(0.5 * (1 + RandomDouble()),0.5 * (1 + RandomDouble()),0.5 * (1 + RandomDouble())),0.5 * RandomDouble()));
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vector3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vector3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture( Vector3(0.4, 0.2, 0.1))));
	list[i++] = new Sphere(Vector3(4, 1, 0), 1.0, new Metal(Vector3(0.7, 0.6, 0.5), 0.0));
	return new BVHNode(list, i, 0, 1);
}

Hittable* PerlinScene() 
{
	Texture* per = new NoiseTexture(3);
	Hittable ** l = new Hittable * [2];
	l[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(per));
	l[1] = new Sphere(Vector3(0, 2, 0), 2, new Lambertian(per));
	return new HittableList(l, 2);
}

Hittable* Light() 
{
	Texture* per = new NoiseTexture(4);
	Hittable** l = new Hittable * [4];
	l[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(per));
	l[1] = new Sphere(Vector3(0, 2, 0), 2, new Lambertian(per));
	l[2] = new Sphere(Vector3(0, 7, 0), 2, new DiffuseLight(new ConstantTexture(Vector3(4,4,4))));
	l[3] = new XYRect(3,5,1,3,-2, new DiffuseLight(new ConstantTexture(Vector3(4,4,4))));
	return new HittableList(l, 4);
}

Hittable* CornellBox() 
{
	Hittable** list = new Hittable * [8];
	Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
	Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
	Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
	Material* light = new DiffuseLight(new ConstantTexture(Vector3(20, 20, 20)));

	list[0] = new FlipNormals(new YZRect (0, 555, 0, 555, 555, green));
	list[1] = new YZRect(0, 555, 0, 555, 0, red);
	list[2] = new XZRect(213, 343, 227, 332, 554, light);
	list[3] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[4] = new XZRect(0, 555, 0, 555, 0, white);
	list[5] = new FlipNormals(new XYRect(0,555,0,555,555,white));
	list[6] = new Translate(new RotateY(new Box(Vector3(0,0,0), Vector3(165,165,165), white),-18),Vector3(130,0,65));
	list[7] = new Translate(new RotateY (new Box(Vector3(0,0,0), Vector3(165,330,165), white),15),Vector3(265, 0, 295));

	return new HittableList(list, 8);
}

Hittable* CornellSmoke() 
{
	Hittable** list = new Hittable * [8];
	Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
	Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
	Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
	Material* light = new DiffuseLight(new ConstantTexture(Vector3(20, 20, 20)));

	list[0] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	list[1] = new YZRect(0, 555, 0, 555, 0, red);
	list[2] = new XZRect(213, 343, 227, 332, 554, light);
	list[3] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[4] = new XZRect(0, 555, 0, 555, 0, white);
	list[5] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
	Hittable* b1 = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 165, 165), white), -18), Vector3(130, 0, 65));
	Hittable* b2= new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(165, 330, 165), white), 15), Vector3(265, 0, 295));
	list[6] = new ConstantMedium(b1, 0.01, new ConstantTexture(Vector3(1.0, 1.0, 1.0)));
	list[7] = new ConstantMedium(b2, 0.01, new ConstantTexture(Vector3(0.0, 0.0, 0.0)));
	return new HittableList(list, 8);
}

Vector3 Color(const Ray& r, Hittable* world, int depth)
{
	HitRecord temp;
	if (world->Hit(r, 0.001, FLT_MAX, temp)) {
		Vector3 attenuation;
		Ray scatter;
		Vector3 emitted = temp.material->Emitted(temp.u,temp.v,temp.p);
		if (depth < 10 && temp.material->Scatter(r, temp, attenuation, scatter))
		{
			return emitted + attenuation * Color(scatter, world, depth + 1);
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return Vector3(0, 0, 0);
		/*Vector3 unit_direction = UnitVector(r.Direction());
		float t = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - t) * Vector3(1.0, 1.0, 1.0) + t * Vector3(0.5, 0.7, 1.0);*/
	}
}

void Save()
{
	std::ofstream Outfile("MyTest.txt", std::ios::out);
	Outfile << "P3\n" << width << " " << height << "\n255\n";

	for (int j = height - 1; j >= 0; j--)
		for (int i = 0, k = 0; i < rgbWidth; i += 3, k++)
		{
			Outfile << pixelData[j * rgbWidth + (i + 0)] << " " << pixelData[j * rgbWidth + (i + 1)] << " " << pixelData[j * rgbWidth + (i + 2)] << "\n";
		}

	Outfile.close();
}
void SetPixelData() 
{
	int dataLength = width * 3;
	while (dataLength % 3 != 0)
		++dataLength;
	dataLength *= height;
	pixelData = new int[dataLength];
}
void Compute(int start,int end) 
{
	for (int j = start; j >= end; j--) {
		std::cout << j << "\n";
		for (int i = 0, k = 0; i < rgbWidth; i += 3, k++) {
			Vector3 color(0, 0, 0);
			for (int s = 0; s < samplingRate; s++)
			{
				float u = (float)(k + RandomDouble()) / (float)width;
				float v = (float)(j + RandomDouble()) / (float)height;
				Ray r = camera.GetRay(u, v);
				color += Color(r, world, 0);
			}
			color /= float(samplingRate);
			color = Vector3(sqrt(color[0]), sqrt(color[1]), sqrt(color[2]));//反射能量减弱 颜色减弱
			pixelData[j * rgbWidth + (i + 0)] = int(255.99 * color[0]);
			pixelData[j * rgbWidth + (i + 1)] = int(255.99 * color[1]);
			pixelData[j * rgbWidth + (i + 2)] = int(255.99 * color[2]);
		}
	}
}
void Func1()
{
	Compute(height- 1, (height * 9) / 10);
}
void Func2()
{
	Compute((height * 9) /10 - 1, (height * 4) / 5);
}
void Func3()
{
	Compute((height * 4) / 5 - 1, (height*7)/10);
}
void Func4()
{
	Compute((height * 7) / 10 -1, (height * 3) / 5);
}
void Func5()
{
	Compute((height * 3) / 5 - 1, height / 2);
}
void Func6()
{
	Compute(height / 2 - 1, (height * 2) / 5);
}
void Func7()
{
	Compute((height * 2) / 5 - 1, (height * 3) / 10);
}
void Func8()
{
	Compute((height * 3) / 10 - 1, height / 5);
}
void Func9()
{
	Compute(height / 5 - 1, height/ 10);
}
void Func10()
{
	Compute(height / 10 - 1, 0);
}
int main()
{
	SetPixelData();
	world = CornellBox();


	std::thread t1(Func1);
	std::thread t2(Func2);
	std::thread t3(Func3);
	std::thread t4(Func4);
	std::thread t5(Func5);
	std::thread t6(Func6);
	std::thread t7(Func7);
	std::thread t8(Func8);
	std::thread t9(Func9);
	std::thread t10(Func10);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();
	t9.join();
	t10.join();

	Save();
	return 0;

}

