#ifndef SPHEREH
#define SPHEREH

#include"hittable.h"


class Sphere :public Hittable
{
public:
	Sphere() {};
	Sphere(Vec3 cen, float t, Material* m) :center(cen), radius(t), material(m) {}
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box)const;
	Vec3 center;
	float radius;
	Material* material;
};
bool Sphere::Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const
{
	Vec3 oc = r.Origin() - center;
	float a = dot(r.Direction(), r.Direction());
	float b = dot(oc, r.Direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < max && temp > min) {
			hitRecord.t = temp;
			hitRecord.p = r.PointAtParameter(hitRecord.t);
			hitRecord.normal = (hitRecord.p - center) / radius;
			hitRecord.material = material;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < max && temp > min) {
			hitRecord.t = temp;
			hitRecord.p = r.PointAtParameter(hitRecord.t);
			hitRecord.normal = (hitRecord.p - center) / radius;
			hitRecord.material = material;
			return true;
		}
	}
	return false;
}
bool Sphere::BoundingBox(float t1,float t2,AABB & box)const
{
	box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
	return true;
}

class HittableList :public Hittable
{
public:
	HittableList() {};
	HittableList(Hittable** l, int s) { list = l; size = s; };
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord)const;
	virtual bool BoundingBox(float t0, float t1, AABB& box)const = 0;
	Hittable** list;
	int size;
};
bool HittableList::Hit(const Ray& r, float min, float max, HitRecord& hitRecord)const
{
	HitRecord tempRecord;
	bool HitAnything = false;
	double closeSoFar = max;
	for (int i = 0; i < size; i++) {
		if (list[i]->Hit(r, min, closeSoFar, tempRecord)) {
			HitAnything = true;
			closeSoFar = tempRecord.t;
			hitRecord = tempRecord;
		}
	}
	return HitAnything;

}
bool HittableList::BoundingBox(float t0,float t1,AABB & box)const
{
	if (size < 1)return false;
	AABB tempBox;
	bool first = list[0]->BoundingBox(t0, t1, tempBox);
	if (!first)return false;
	box = tempBox;
	for (int i = 1; i < size; i++)
	{
		if (list[i]->BoundingBox(t0, t1, tempBox))
		{
			box = SurrondBox(box,tempBox);
		}
		else return false;
	}
	return true;
}

class BVHNode 
{
public:
	BVHNode() {}
	BVHNode(Hittable ** l,int n ,float t0,float t1) {}

	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const;
	virtual bool BoundingBox(float t0, float t1, AABB& b)const;

	Hittable* left;
	Hittable* right;
	AABB box;
};
bool BVHNode::Hit(const Ray& r, float min, float max, HitRecord& hitRecord)const 
{
	if (box.Hit(r, min, max))
	{
		HitRecord leftRecord, rightRecord;
		bool hitLeft = left->Hit(r, min, max, leftRecord);
		bool hitRight = right->Hit(r, min, max, rightRecord);
		if (hitLeft && hitRight)
		{
			if (leftRecord.t < rightRecord.t)
				hitRecord = leftRecord;
			else hitRecord = rightRecord;
		}
		else if (hitLeft)
		{
			hitRecord = leftRecord;
		}
		else if (hitRight)
		{
			hitRecord = rightRecord;
		}
		else return false;
	}
	else return false;
	return true;

}

bool BVHNode::BoundingBox(float t0, float t1, AABB& b)const
{
	b = box;
	return true;
}
#endif