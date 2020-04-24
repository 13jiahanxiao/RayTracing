#ifndef TEXTUREH
#define TEXTUREH

#include"vector.h"
#include"noise.h"

class Texture 
{
public :
	virtual Vector3 Value(float u, float v, const Vector3&p)const = 0;
};

class ConstantTexture :public Texture 
{
public:
	ConstantTexture() {}
	ConstantTexture(Vector3 a) { color = a; }
	virtual Vector3 Value(float u, float v, const Vector3& p) const
	{
		return color;
	};

	Vector3 color;
};

class TestTexture :public Texture 
{
public :
	TestTexture() {};
	TestTexture(Texture * a,Texture * b) :odd(a),even(b) {};
	virtual Vector3 Value(float u, float v, const Vector3& p)const  
	{
		float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sines > 0)return odd->Value(u, v, p);
		else return even->Value(u, v, p);
	}

	Texture* odd;
	Texture* even;
};

class NoiseTexture :public Texture
{
public:
	NoiseTexture() {};
	NoiseTexture(float sc) :scale(sc) {};
	virtual Vector3 Value(float u, float v, const Vector3& p)const { return Vector3(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.Turb(p))); };

	Perlin noise;
	float scale;
};

#endif // !TEXTUREH