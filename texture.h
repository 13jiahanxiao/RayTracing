#ifndef TEXTUREH
#define TEXTUREH

#include"vector.h"
#include"noise.h"

class Texture 
{
public :
	virtual Vec3 Value(float u, float v, const Vec3&p)const = 0;
};

class ConstantTexture :public Texture 
{
public:
	ConstantTexture() {}
	ConstantTexture(Vec3 a) { color = a; }
	virtual Vec3 Value(float u, float v, const Vec3& p) const
	{
		return color;
	};

	Vec3 color;
};

class TestTexture :public Texture 
{
public :
	TestTexture() {};
	TestTexture(Texture * a,Texture * b) :odd(a),even(b) {};
	virtual Vec3 Value(float u, float v, const Vec3& p)const  
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
	virtual Vec3 Value(float u, float v, const Vec3& p)const { return Vec3(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.Turb(p))); };

	Perlin noise;
	float scale;
};

#endif // !TEXTUREH