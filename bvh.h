#ifndef BVHH
#define BVHH
#include"vector.h"
#include"ray.h"
class AABB
{
public:
	AABB() {}
	AABB(const Vector3& a, const Vector3& b) 
	{
		min = a;
		max = b;
	}

	Vector3 Min() const { return min; }
	Vector3 Max() const { return max; }
	bool Hit (const Ray& r, float tMin, float tMax) const
	{
		for  (int i = 0; i <3 ; i++)
		{
			float invD = 1.0f / r.Direction()[i];
			float t0 = (min[i] - r.Origin()[i]) * invD;
			float t1 = (max[i] - r.Origin()[i]) * invD;
			if (invD < 0.0f)
			{
				std::swap(t0, t1);
			}

			tMin = t0 > tMin ? t0 : tMin;
			tMax = t1 < tMax ? t1 : tMax;

			if (tMax <= tMin)
				return false;
		}
		return true;
	}

	Vector3 min;
	Vector3 max;
};

AABB SurrondBox(AABB b1, AABB b2) 
{
	Vector3 small
	{
		std::fmin(b1.Min().x(),b2.Min().x()),
		std::fmin(b1.Min().y(),b2.Min().y()),
		std::fmin(b1.Min().z(),b2.Min().z())
	};
	Vector3 big
	{
		std::fmax(b1.Max().x(),b2.Max().x()),
		std::fmax(b1.Max().y(),b2.Max().y()),
		std::fmax(b1.Max().z(),b2.Max().z())
	};
	return AABB(small, big);

}
#endif // !BVHH


