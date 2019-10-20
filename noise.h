#ifndef NOISEH
#define NOISEH

#include"vector.h"
#include"random.h"
class Perlin
{
public :
	float Noise(Vec3& p) 
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());

		int i = int(4 * p.x()) & 255;
		int j = int(4 * p.y()) & 255;
		int k = int(4*p.z()) & 255;
		return ranfloat
	}

	static float* ranFloat;
	static int* permuteX;
	static int* permuteY;
	static int* permuteZ;
};

static float* PerlinGenerate() 
{
	float* p = new float[256];
	for(int i=0;i<256;i++)
	{
		p[i] = RandomFloat();
	}
	return p;
}

void Permute(float* p, int n) 
{
	for (int i = n - 1; i > 0; i--) 
	{
		target = int(RandomFloat() * i);
		float temp = p[i];
		p[i] = p[target];
		p[target] = temp;
	}
}

static int* PerlinGeneratePermute() 
{
	int* p = new  int[256];
	for (int i = 0; i < 256; i++) 
	{
		p[i] = i;
	}
	Permute(p, 256);
	return p;
}

float* Perlin::ranFloat = PerlinGenerate();
int* Perlin::permuteX = PerlinGeneratePermute();
int* Perlin::permuteY = PerlinGeneratePermute();
int* Perlin::permuteZ = PerlinGeneratePermute();

#endif // !NOISEH

