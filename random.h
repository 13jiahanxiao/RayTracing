#ifndef RANDOMH
#define RANDOMH

#include <random>
#include <functional>
#define M_PI       3.14159265358979323846
double RandomDouble()
{
	/*static std::uniform_real_distribution<double> distribution(0, 1);
	static std::mt19937 generator;
	static std::function<double()> randGenerator = std::bind(distribution, generator);
	return randGenerator();*/
	return rand() / (RAND_MAX + 1.0);
}
Vector3 RandomInUnitSphere()
{
	Vector3 p;
	do {
		p = 2.0 * Vector3(RandomDouble(), RandomDouble(), RandomDouble()) - Vector3(1, 1, 1);
	} while (p.SquaredLength() >= 1);
	return p;
}
Vector3 RandomInUnitDisk() {
	Vector3 p;
	do {
		p = 2.0 * Vector3(RandomDouble(), RandomDouble(), 0) - Vector3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return UnitVector(p);
}

inline Vector3 RandomCosineDirection() {
	float r1 = RandomDouble();
	float r2 = RandomDouble();
	float z = sqrt(1 - r2);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * sqrt(r2);
	float y = sin(phi) * sqrt(r2);
	return Vector3(x, y, z);
}

inline Vector3 RandomToSphere(float radius, float distance_squared) {
	float r1 = RandomDouble();
	float r2 = RandomDouble();
	float z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);
	float phi = 2 * M_PI * r1;
	float x = cos(phi) * sqrt(1 - z * z);
	float y = sin(phi) * sqrt(1 - z * z);
	return Vector3(x, y, z);
}



//Ortho-normal Bases
class  ONB 
{
public:
	ONB (){}
	Vector3 operator[](int i)const { return axis[i]; }
	Vector3 U() const { return axis[0]; };
	Vector3 V() const { return axis[1]; };
	Vector3 W() const { return axis[2]; };
	Vector3 Local(float a, float b, float c)const { return a * U() + b * V() + c * W(); }
	Vector3 Local(const Vector3& a)const { return a.x() * U() + a.y() * V() + a.z() * W(); }
	void BuildFromW(const Vector3&);
	Vector3 axis[3];
};

void ONB::BuildFromW(const Vector3& n) {
	axis[2] = UnitVector(n);
	Vector3 a;
	if (fabs(W().x()) > 0.9)
		a = Vector3(0, 1, 0);
	else
		a = Vector3(1, 0, 0);
	axis[1] = UnitVector(cross(W(), a));
	axis[0] = cross(W(), V());
}

#endif
