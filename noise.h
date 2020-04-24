#ifndef NOISEH
#define NOISEH

#include"vector.h"
#include"random.h"

float TrilinearInterp(Vector3 c[2][2][2], float u, float v,float w)
{
	float uu = u * u * (3 - 2 * u);
	float vv = v * v * (3 - 2 * v);
	float ww = w * w * (3 - 2 * w);
	float accum = 0;
	for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
			for (int k = 0; k < 2; k++)
			{
				Vector3 weightV(u - i, v - j, w - k);
				accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weightV);
			}
	return accum;
}

class Perlin
{
public :
	float Noise(const Vector3& p) const
	{
		float u = p.x() - floor(p.x());
		float v = p.y() - floor(p.y());
		float w = p.z() - floor(p.z());

		int i =floor(p.x());
		int j =floor(p.y());
		int k =floor(p.z());

		Vector3 c[2][2][2];
		for (int di = 0; di < 2; di++)
			for (int dj = 0; dj < 2; dj++)
				for (int dk = 0; dk < 2; dk++)
					c[di][dj][dk] = ranVec[permuteX[(i + di) & 255] ^permuteY[(j + dj) & 255] ^permuteZ[(k + dk) & 255]];
		return TrilinearInterp(c, u, v, w);
	}
	float Turb(const Vector3& p, int depth = 7)const
	{
		float accum = 0.0f;
		Vector3 temp = p;
		float weight = 1.0f;
		for (int i = 0; i < depth; i++)
		{
			accum += weight * Noise(temp);
			weight *= 0.5f;
			temp *= 2;
		}

		return fabs(accum);
	}

	static Vector3* ranVec;
	static int* permuteX;
	static int* permuteY;
	static int* permuteZ;
};
static Vector3* PerlinGenerate() 
{
	Vector3* p = new Vector3[256];
	for (int i = 0; i < 256; i++) {
		p[i] = UnitVector(Vector3(2 * RandomDouble() - 1, 2 * RandomDouble() - 1, 2 * RandomDouble() - 1));
	}
	return p;
}
void Permute(int* p, int n) 
{
	for (int i = n - 1; i > 0; i--) 
	{
		int target = int(RandomDouble() * (i + 1));
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

Vector3* Perlin::ranVec = PerlinGenerate();
int* Perlin::permuteX = PerlinGeneratePermute();
int* Perlin::permuteY = PerlinGeneratePermute();
int* Perlin::permuteZ = PerlinGeneratePermute();

#endif // !NOISEH

