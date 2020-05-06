#include <iostream>
#include <thread>
#include<fstream>
#include"vector.h"
#include"ray.h"
#include"hittable.h"
#include"camera.h"
#include"material.h"
#include"geometry.h"
#include"pdf.h"

int width = 1200;
int height = 900;
int rgbWidth = width * 3;
int samplingRate = 1000;

int* pixelData;

Hittable* world;
Camera* camera;
Hittable* sampleList;

//Hittable* RandomScene() {
//	int n = 10;
//	Hittable** list = new Hittable * [n + 1];
//	Texture* test = new TestTexture{ new ConstantTexture(Vector3(0.2,0.3,0.1)),new ConstantTexture(Vector3( 0.9,0.9,0.9)) };
//	list[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(test));
//	int i = 1;
//	for (int a = -1; a <1; a++) {
//		for (int b = -1; b < 1; b++) {
//			float chooseMat = RandomDouble();
//			Vector3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());
//			if ((center - Vector3(4, 0.2, 0)).Length() > 0.9) {
//				if (chooseMat < 0.8) {  // diffuse
//					list[i++] = new Sphere(center, 0.2,new Lambertian(new ConstantTexture( Vector3(RandomDouble() * RandomDouble(),RandomDouble() * RandomDouble(),RandomDouble() * RandomDouble()))));
//				}
//				else if (chooseMat < 0.95) { // metal
//					list[i++] = new Sphere(center, 0.2,	new Metal(Vector3(0.5 * (1 + RandomDouble()),0.5 * (1 + RandomDouble()),0.5 * (1 + RandomDouble())),0.5 * RandomDouble()));
//				}
//				else {  // glass
//					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
//				}
//			}
//		}
//	}
//
//	list[i++] = new Sphere(Vector3(0, 1, 0), 1.0, new Dielectric(1.5));
//	list[i++] = new Sphere(Vector3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture( Vector3(0.4, 0.2, 0.1))));
//	list[i++] = new Sphere(Vector3(4, 1, 0), 1.0, new Metal(Vector3(0.7, 0.6, 0.5), 0.0));
//	return new BVHNode(list, i, 0, 1);
//}
//
//Hittable* PerlinScene() 
//{
//	Texture* per = new NoiseTexture(3);
//	Hittable ** l = new Hittable * [2];
//	l[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(per));
//	l[1] = new Sphere(Vector3(0, 2, 0), 2, new Lambertian(per));
//	return new HittableList(l, 2);
//}
//
//Hittable* Light() 
//{
//	Hittable** list = new Hittable * [6];
//	Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
//	Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
//	Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
//	Material* light = new DiffuseLight(new ConstantTexture(Vector3(15, 15, 15)));
//
//	list[0] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
//	list[1] = new YZRect(0, 555, 0, 555, 0, red);
//	list[2] = new XZRect(213, 343, 227, 332, 554, light);
//	list[3] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
//	list[4] = new XZRect(0, 555, 0, 555, 0, white);
//	list[5] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
//	return new HittableList(list,6);
//}
//Hittable* CornellSmoke()
//{
//	Hittable** list = new Hittable * [7];
//	Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
//	Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
//	Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
//	Material* light = new DiffuseLight(new ConstantTexture(Vector3(25, 25, 25)));
//
//	list[0] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
//	list[1] = new YZRect(0, 555, 0, 555, 0, red);
//	list[2] = new XZRect(213, 343, 227, 332, 554, light);
//	list[3] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
//	list[4] = new XZRect(0, 555, 0, 555, 0, white);
//	list[5] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));
//	Hittable* b1 = new Translate(new Box(Vector3(0, 0, 0), Vector3(555, 50, 555), white), Vector3(0, 50, 0));
//	list[6] = new ConstantMedium(b1, 0.005, new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
//	return new HittableList(list, 7);
//}
Vector3 denan(const Vector3& c) {
	Vector3 temp = c;
	if (!(temp[0] == temp[0])) temp[0] = 0;
	if (!(temp[1] == temp[1])) temp[1] = 0;
	if (!(temp[2] == temp[2])) temp[2] = 0;
	return temp;
}

void CornellBox(Hittable** scene, Camera** cam)
{
	Vector3 lookfrom(278, 278, -800);
	Vector3 lookat(278, 278, 0);
	float distToFocus = 10;
	float aperture = 0.0;
	float vFov = 40.0;
	*cam=new Camera(lookfrom, lookat, Vector3(0, 1, 0), vFov, float(width) / float(height), aperture, distToFocus, 0, 1);
	
	Hittable** list = new Hittable * [8];
	Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
	Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
	Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
	Material* light = new DiffuseLight(new ConstantTexture(Vector3(15, 15, 15)));

	list[0] = new FlipNormals(new YZRect (0, 555, 0, 555, 555, green));
	list[1] = new YZRect(0, 555, 0, 555, 0, red);
	list[2] = new FlipNormals( new XZRect(213, 343, 227, 332, 554, light));
	list[3] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[4] = new XZRect(0, 555, 0, 555, 0, white);
	list[5] = new FlipNormals(new XYRect(0,555,0,555,555,white));
	list[6] = new Translate(new RotateY(new Box(Vector3(0,0,0), Vector3(165,165,165), white),-18),Vector3(130,0,65));
	list[7] = new Translate(new RotateY (new Box(Vector3(0,0,0), Vector3(165,330,165), white),15),Vector3(265, 0, 295));

	*scene=new HittableList(list, 8);
}
void MyBox(Hittable** scene, Camera** cam)
{
	Vector3 lookfrom(76, 60, -180);
	Vector3 lookat(76, 60, 0);
	float distToFocus = 10;
	float aperture = 0.0;
	float vFov = 40.0;
	*cam=new Camera(lookfrom, lookat, Vector3(0, 1, 0), vFov, float(width) / float(height), aperture, distToFocus, 0, 1);

	Hittable** list = new Hittable * [8];
	Material* red = new Lambertian(new ConstantTexture(Vector3(0.65, 0.05, 0.05)));
	Material* white = new Lambertian(new ConstantTexture(Vector3(0.73, 0.73, 0.73)));
	Material* green = new Lambertian(new ConstantTexture(Vector3(0.12, 0.45, 0.15)));
	Material* light = new DiffuseLight(new ConstantTexture(Vector3(16, 16, 16)));
	Material* alumium = new Metal(Vector3(0.8, 0.85, 0.88), 0.0);
	Material* glass = new Dielectric(1.5);

	list[0] = new FlipNormals(new YZRect(0, 120, 0,100, 160, green));
	list[1] = new YZRect(0, 120, 0, 100, 0, red);
	list[2] = new FlipNormals(new XZRect(65, 95, 35, 65, 119, light));
	list[3] = new FlipNormals(new XZRect(0, 160, 0, 100, 120, white));
	list[4] = new XZRect(0, 160, 0, 100, 0, white);
	list[5] = new FlipNormals(new XYRect(0, 160, 0, 120, 100, white));
	list[6] = new Translate(new Sphere(Vector3(20, 20, 20), 20, glass), Vector3(25, 0, 20));
	list[7] = new Translate(new RotateY(new Box(Vector3(0, 0, 0), Vector3(40, 80, 40), alumium), 35), Vector3(80, 0, 55));

	*scene=new HittableList(list, 8);
}
void SampleList(Hittable** sample)
{
	Hittable** a=new Hittable*[2];
	Hittable* lightShape = new XZRect(70, 90, 40, 60, 120, nullptr);
	Hittable* glassSphere = new Sphere(Vector3(190, 90, 190), 90, 0);
	a[0] = lightShape;
	a[1] = glassSphere;
	*sample=new HittableList(a, 2);
}
Vector3 Color(const Ray& r, Hittable* world,Hittable *lightShape, int depth)
{
	HitRecord temp;
	if (world->Hit(r, 0.001, FLT_MAX, temp)) {
		ScatterRecord srec;
		Vector3 emitted = temp.material->Emitted(r,temp,temp.u,temp.v,temp.p);
		if (depth < 10 && temp.material->Scatter(r, temp,srec))
		{
			if (srec.isSpecular) 
			{
				return srec.attenuation * Color(srec.specularRay, world, lightShape, depth + 1);
			}
			else {
				HittablePDF p0(lightShape, temp.p);
				MixturePDF p(&p0, srec.pdfPtr);
				Ray scatter = Ray(temp.p, p.Generate(), r.Time());
				float pdf = p.Value(scatter.Direction());
				delete srec.pdfPtr;
				return emitted + srec.attenuation * temp.material->ScatteringPDF(r, temp, scatter) * Color(scatter, world,lightShape,depth + 1) / pdf;
			}
		}
		else
		{
			return emitted;
		}
	}
	else
	{
		return Vector3(0, 0, 0);
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
			//std::cout << j <<" "<<k<< "\n";
			Vector3 color(0, 0, 0);
			for (int s = 0; s < samplingRate; s++)
			{
				float u = (float)(k + RandomDouble()) / (float)width;
				float v = (float)(j + RandomDouble()) / (float)height;
				Ray r = camera->GetRay(u, v);
				color +=denan(Color(r, world,sampleList, 0));
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
void MultipleProcess() 
{
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
}

int main()
{
	SetPixelData();
	MyBox(&world,&camera);
	SampleList(&sampleList);
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
	//MultipleProcess();
    //Compute(80 - 1, 0);
	Save();
	return 0;

}

