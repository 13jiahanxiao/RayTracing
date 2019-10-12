#ifndef SPHEREH
#define SPHEREH

#include"hittable.h"


class Sphere :public Hittable
{
public:
	Sphere() {};
	Sphere(Vec3 cen, float t, Material* m) :center(cen), radius(t), material(m) {}
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const;
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

class HittableList :public Hittable
{
public:
	HittableList() {};
	HittableList(Hittable** l, int s) { list = l; size = s; };
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord)const;
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
#endif