#include <iostream>
#include<fstream>

#pragma region Vector3

class Vec3 {
public:
	Vec3() {}
	Vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }
	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline Vec3& operator+=(const Vec3& v2);
	inline Vec3& operator-=(const Vec3& v2);
	inline Vec3& operator*=(const Vec3& v2);
	inline Vec3& operator/=(const Vec3& v2);
	inline Vec3& operator*=(const float t);
	inline Vec3& operator/=(const float t);

	inline float Length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
	inline float LquaredLength() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	inline void MakeUnitVector();

	float e[3];
};

inline std::istream& operator>>(std::istream& is, Vec3& t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline void Vec3::MakeUnitVector() {
	float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	e[0] *= k; e[1] *= k; e[2] *= k;
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3 operator*(float t, const Vec3& v) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, float t) {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator/(Vec3 v, float t) {
	return Vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline float dot(const Vec3& v1, const Vec3& v2) {
	return v1.e[0] * v2.e[0]
		+ v1.e[1] * v2.e[1]
		+ v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
		v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
		v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vec3& Vec3::operator+=(const Vec3& v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
	float k = 1.0 / t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vec3 unitVector(Vec3 v) {
	return v / v.Length();
}
#pragma endregion

#pragma region Ray

class Ray 
{
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b) { A = a; B = b; }
	Vec3 Origin() const{ return A; }
	Vec3 Direction() const{ return B; }
	Vec3 PointAtParameter(float t) { return A + t * B; }
private:
	Vec3 A;
	Vec3 B;
 };

#pragma endregion

bool HitSphere(const Vec3& origin, float radius ,const Ray& r) 
{
	Vec3 oc = r.Origin() - origin;
	float a = dot(r.Direction(), r.Direction());
	float b = 2.0f * dot(r.Direction(), oc);
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

Vec3 Color(const Ray& r) 
{
	if (HitSphere(Vec3(0, 0, -3), 2, r)) {
		return Vec3(1, 0, 0);
	}
	Vec3 unitDirection = unitVector(r.Direction());
	float t = 0.5f * (unitDirection.y() + 1);
	return (1 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1);
}

int main() 
{
	int nx = 1280;
	int ny = 720;
	Vec3 lowerLeftCorner(-8, -4.5f, -3);
	Vec3 horizontal(16, 0, 0);
	Vec3 vertical(0, 9, 0);
	Vec3 origin(0, 0, 0);
	std::ofstream Outfile("MyTest.txt", std::ios::out);
	Outfile << "P3\n" << nx << " " << ny << "\n255\n";
	for(int j=ny-1;j>=0;j--)
		for (int i = 0; i < nx; i++) {
			float u= (float)i / (float)nx;
			float v= (float)j / (float)ny;
			Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
			Vec3 color= Color(r);
			int ir = (int)255.99 * color[0];
			int ig = (int)255.99 * color[1];
			int ib = (int)255.99 * color[2];
			Outfile << ir << " " << ig << " " << ib << "\n";
		}
}