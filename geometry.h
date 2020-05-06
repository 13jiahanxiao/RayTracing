#ifndef GEOMETRYH
#define GEOMETRYH

#include"hittable.h"

int BoxXCompare(const void* a, const void* b)
{
	AABB leftBox, rightBox;
	Hittable* lb = *(Hittable * *)a;
	Hittable* rb = *(Hittable * *)b;
	if (!lb->BoundingBox(0, 0, leftBox) || !rb->BoundingBox(0, 0, rightBox))
		std::cerr << "no bounding box in BVHNode constructor \n";
	if (leftBox.Min().x() - rightBox.Min().x() < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
int BoxYCompare(const void* a, const void* b)
{
	AABB leftBox, rightBox;
	Hittable* lb = *(Hittable * *)a;
	Hittable* rb = *(Hittable * *)b;
	if (!lb->BoundingBox(0, 0, leftBox) || !rb->BoundingBox(0, 0, rightBox))
		std::cerr << "no bounding box in BVHNode constructor \n";
	if (leftBox.Min().y() - rightBox.Min().y() < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}
int BoxZCompare(const void* a, const void* b)
{
	AABB leftBox, rightBox;
	Hittable* lb = *(Hittable * *)a;
	Hittable* rb = *(Hittable * *)b;
	if (!lb->BoundingBox(0, 0, leftBox) || !rb->BoundingBox(0, 0, rightBox))
		std::cerr << "no bounding box in BVHNode constructor \n";
	if (leftBox.Min().z() - rightBox.Min().z() < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

class Sphere :public Hittable
{
public:
	Sphere() {};
	Sphere(Vector3 cen, float t, Material* m) :center(cen), radius(t), material(m) {}
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box)const;
	float PDFValue(const Vector3& o, const Vector3& v)const 
	{
		HitRecord rec;
		if (this->Hit(Ray(o, v), 0.001, FLT_MAX, rec)) {
			float cosThetaMax = sqrt(1 - radius * radius / (center - o).SquaredLength());
			float solidAngle = 2 * M_PI * (1 - cosThetaMax);
			return  1 / solidAngle;
		}
		else
			return 0;
	}
	Vector3 Random(const Vector3& o) const {
		Vector3 direction = center - o;
		float distanceSquared = direction.SquaredLength();
		ONB uvw;
		uvw.BuildFromW(direction);
		return uvw.Local(RandomToSphere(radius, distanceSquared));
	}
	Vector3 center;
	float radius;
	Material* material;
};
bool Sphere::Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const
{
	Vector3 oc = r.Origin() - center;
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
	box = AABB(center - Vector3(radius, radius, radius), center + Vector3(radius, radius, radius));
	return true;
}

class HittableList :public Hittable
{
public:
	HittableList() {};
	HittableList(Hittable** l, int s) { list = l; size = s; };
	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord)const;
	virtual bool BoundingBox(float t0, float t1, AABB& box)const ;
	float PDFValue(const Vector3& o, const Vector3& v) const {
		float weight = 1.0 / size;
		float sum = 0;
		for (int i = 0; i < size; i++)
			sum += weight * list[i]->PDFValue(o, v);
		return sum;
	}

	Vector3 Random(const Vector3& o) const {
		int index = int(RandomDouble() * size);
		return list[index]->Random(o);
	}
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

class BVHNode :public Hittable{
public:
	BVHNode() {}
	BVHNode(Hittable** l, int n, float t0, float t1);

	virtual bool Hit(const Ray& r, float min, float max, HitRecord& hitRecord) const;
	virtual bool BoundingBox(float t0, float t1, AABB& b)const;

	Hittable* left;
	Hittable* right;
	AABB box;
};
BVHNode::BVHNode(Hittable** l, int n, float t0, float t1)
{
	int anix = int(3 * RandomDouble());
	if (anix == 0) qsort(l, n, sizeof(Hittable*), BoxXCompare);
	else if (anix == 1)qsort(l, n, sizeof(Hittable*), BoxYCompare);
	else qsort(l, n, sizeof(Hittable*), BoxZCompare);

	if (n == 1) { left = right = l[0]; }
	else if (n == 2) { left = l[0], right = l[1]; }
	else
	{
		left = new BVHNode(l, n / 2, t0, t1);
		right = new BVHNode(l + n / 2, n - n / 2, t0, t1);
	}
	AABB boxLeft, boxRight;
	if (!left->BoundingBox(t0, t1, boxLeft) || !right->BoundingBox(t0, t1, boxRight))
	{
		std::cerr << "no bounding box in BVHNode constructor \n";
	}
	box = SurrondBox(boxLeft, boxRight);
}
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
			return true;
		}
		else if (hitLeft)
		{
			hitRecord = leftRecord;
			return true;;
		}
		else if (hitRight)
		{
			hitRecord = rightRecord;
			return true;
		}
		else 
			return false;
	}
    return false;
}
bool BVHNode::BoundingBox(float t0, float t1, AABB& b)const
{
	b = box;
	return true;
}

class XYRect : public Hittable {
public:
	XYRect() {}
	XYRect(float _x0, float _x1, float _y0, float _y1, float _k, Material* mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const {
		box = AABB(Vector3(x0, y0, k - 0.0001), Vector3(x1, y1, k + 0.0001));
		return true;
	}	
	Material* mp;
	float x0, x1, y0, y1, k;
};
bool XYRect::Hit(const Ray& r, float t0, float t1,HitRecord& rec) const {
	float t = (k - r.Origin().z()) / r.Direction().z();
	if (t < t0 || t > t1)
		return false;
	float x = r.Origin().x() + t * r.Direction().x();
	float y = r.Origin().y() + t * r.Direction().y();
	if (x < x0 || x > x1 || y < y0 || y > y1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (y - y0) / (y1 - y0);
	rec.t = t;
	rec.material = mp;
	rec.p = r.PointAtParameter(t);
	rec.normal = Vector3(0, 0, 1);
	return true;
}

class XZRect : public Hittable {
public:
	XZRect() {}
	XZRect(float _x0, float _x1, float _z0, float _z1, float _k, Material* mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const {
		box = AABB(Vector3(x0, k - 0.0001,z0), Vector3(x1,k + 0.0001,z1));
		return true;
	}
	virtual float PDFValue(const Vector3& o, const Vector3& v)const
	{
		HitRecord rec;
		if (this->Hit(Ray(o, v), 0.001, FLT_MAX, rec)) {
			float area = (x1 - x0) * (z1 - z0);
			float distanceSquared = rec.t * rec.t * v.SquaredLength();
			float cosine = fabs(dot(v, rec.normal) / v.Length());
			return  distanceSquared / (cosine * area);
		}
		else
			return 0;
	}
	virtual Vector3 Random(const Vector3& o) const {
		Vector3 randomPoint = Vector3(x0 + RandomDouble() * (x1 - x0), k,z0 + RandomDouble() * (z1 - z0));
		return randomPoint - o;
	}
	Material* mp;
	float x0, x1, z0, z1, k;
};
bool XZRect::Hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
	float t = (k - r.Origin().y()) / r.Direction().y();
	if (t < t0 || t > t1)
		return false;
	float x = r.Origin().x() + t * r.Direction().x();
	float z = r.Origin().z() + t * r.Direction().z();
	if (x < x0 || x > x1 || z < z0 || z > z1)
		return false;
	rec.u = (x - x0) / (x1 - x0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.material = mp;
	rec.p = r.PointAtParameter(t);
	rec.normal = Vector3(0, 1, 0);
	return true;
}

class YZRect : public Hittable {
public:
	YZRect() {}
	YZRect(float _y0, float _y1, float _z0, float _z1, float _k, Material* mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {};
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const {
		box = AABB(Vector3(k - 0.0001, y0, z0), Vector3(k + 0.0001, y1, z1));
		return true;
	}
	Material* mp;
	float y0, y1, z0, z1, k;
};
bool YZRect::Hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
	float t = (k - r.Origin().x()) / r.Direction().x();
	if (t < t0 || t > t1)
		return false;
	float y = r.Origin().y() + t * r.Direction().y();
	float z = r.Origin().z() + t * r.Direction().z();
	if (y < y0 || y > y1 || z < z0 || z > z1)
		return false;
	rec.u = (y - y0) / (y1 - y0);
	rec.v = (z - z0) / (z1 - z0);
	rec.t = t;
	rec.material = mp;
	rec.p = r.PointAtParameter(t);
	rec.normal = Vector3(1,0,0);
	return true;
}

class FlipNormals : public Hittable {
public:
	FlipNormals(Hittable* p) : ptr(p) {}

	virtual bool Hit(
		const Ray& r, float t_min, float t_max, HitRecord& rec) const {

		if (ptr->Hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}

	virtual bool BoundingBox(float t0, float t1,AABB& box) const {
		return ptr->BoundingBox(t0, t1, box);
	}

	Hittable* ptr;
};

class Box : public Hittable {
public:
	Box() {}
	Box(const Vector3& p0, const Vector3& p1, Material* ptr);
	virtual bool Hit(const Ray& r, float t0, float t1, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const {
		box = AABB(pmin, pmax);
		return true;
	}
	Vector3 pmin, pmax;
	Hittable* listPtr;
};
Box::Box(const Vector3& p0, const Vector3& p1, Material* ptr) {
	pmin = p0;
	pmax = p1;
	Hittable** list = new Hittable * [6];
	list[0] = new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
	list[1] = new FlipNormals(new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
	list[2] = new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
	list[3] = new FlipNormals(new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
	list[4] = new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
	list[5] = new FlipNormals(new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
	listPtr = new HittableList(list, 6);
}
bool Box::Hit(const Ray& r, float t0, float t1, HitRecord& rec) const {
	return listPtr->Hit(r, t0, t1, rec);
}

class Translate : public Hittable {
public:
	Translate(Hittable* p, const Vector3& displacement): ptr(p), offset(displacement) {}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const;
	Hittable* ptr;
	Vector3 offset;
};
bool Translate::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Ray moved_r(r.Origin() - offset, r.Direction(), r.Time());
	if (ptr->Hit(moved_r, t_min, t_max, rec)) {
		rec.p += offset;
		return true;
	}
	else
		return false;
}
bool Translate::BoundingBox(float t0, float t1, AABB& box) const {
	if (ptr->BoundingBox(t0, t1, box)) {
		box = AABB(box.Min() + offset, box.Max() + offset);
		return true;
	}
	else
		return false;
}

class RotateY : public Hittable {
public:
	RotateY(Hittable* p, float angle);
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	virtual bool BoundingBox(float t0, float t1, AABB& box) const {
		box = bbox; return hasbox;
	}
	Hittable* ptr;
	float sin_theta;
	float cos_theta;
	bool hasbox;
	AABB bbox;
};
RotateY::RotateY(Hittable* p, float angle) : ptr(p) {
	float radians = (M_PI / 180.) * angle;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->BoundingBox(0, 1, bbox);
	Vector3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				float x = i * bbox.Max().x() + (1 - i) * bbox.Min().x();
				float y = j * bbox.Max().y() + (1 - j) * bbox.Min().y();
				float z = k * bbox.Max().z() + (1 - k) * bbox.Min().z();
				float newx = cos_theta * x + sin_theta * z;
				float newz = -sin_theta * x + cos_theta * z;
				Vector3 tester(newx, y, newz);
				for (int c = 0; c < 3; c++)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
		}
	}
	bbox = AABB(min, max);
}
bool RotateY::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Vector3 origin = r.Origin();
	Vector3 direction = r.Direction();
	origin[0] = cos_theta * r.Origin()[0] - sin_theta * r.Origin()[2];
	origin[2] = sin_theta * r.Origin()[0] + cos_theta * r.Origin()[2];
	direction[0] = cos_theta * r.Direction()[0] - sin_theta * r.Direction()[2];
	direction[2] = sin_theta * r.Direction()[0] + cos_theta * r.Direction()[2];
	Ray rotated_r(origin, direction, r.Time());
	if (ptr->Hit(rotated_r, t_min, t_max, rec)) {
		Vector3 p = rec.p;
		Vector3 normal = rec.normal;
		p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
		p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
		normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
		normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else
		return false;
}

//class ConstantMedium :public Hittable 
//{public:
//	ConstantMedium(Hittable* b, float d, Texture* a) :boundary(b), density(d) 
//	{
//		phaseFunction = new Isotropic(a);
//	};
//	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
//	virtual bool BoundingBox(float t0, float t1, AABB& box)const 
//	{
//		return boundary->BoundingBox(t0, t1, box);
//	};
//
//	Hittable* boundary;
//	float density;
//	Material* phaseFunction;
//};

//bool ConstantMedium::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const
//{
//	const bool enableDebug = false;
//	bool debugging = enableDebug && RandomDouble() < 0.00001;
//
//	HitRecord rec1, rec2;
//
//	if (boundary->Hit(r, -FLT_MAX, FLT_MAX, rec1)) {
//		if (boundary->Hit(r, rec1.t + 0.0001, FLT_MAX, rec2)) {
//
//			if (debugging) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << '\n';
//
//			if (rec1.t < t_min)
//				rec1.t = t_min;
//
//			if (rec2.t > t_max)
//				rec2.t = t_max;
//
//			if (rec1.t >= rec2.t)
//				return false;
//
//			if (rec1.t < 0)
//				rec1.t = 0;
//
//			float distance_inside_boundary = (rec2.t - rec1.t) * r.Direction().Length();
//			float hit_distance = -(1 / density) * log(RandomDouble());
//
//			if (hit_distance < distance_inside_boundary) {
//
//				rec.t = rec1.t + hit_distance / r.Direction().Length();
//				rec.p = r.PointAtParameter(rec.t);
//
//				if (debugging) {
//					std::cerr << "hit_distance = " << hit_distance << '\n'
//						<< "rec.t = " << rec.t << '\n'
//						<< "rec.p = " << rec.p << '\n';
//				}
//
//				rec.normal = Vector3(1, 0, 0);  // arbitrary
//				rec.material = phaseFunction;
//				return true;
//			}
//		}
//	}
//	return false;
//}
#endif