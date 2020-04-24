#ifndef HITTABLEH
#define HITTABLEH

#include"vector.h"
#include"ray.h"
#include"bvh.h"

class Material;
class AABB;

struct  HitRecord
{
	float t;
	float u;
	float v;
	Vector3 p;
	Vector3 normal;
	Material *material;
};
class Hittable
{
public:
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const = 0;
	virtual bool BoundingBox(float t0, float t1, AABB& r)const = 0;
};
#endif