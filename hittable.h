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
	virtual float PDFValue(const Vector3& o, const Vector3& v)const { return 0.0; }
	virtual Vector3 Random(const Vector3& o)const { return Vector3(1, 0, 0); }
};
#endif