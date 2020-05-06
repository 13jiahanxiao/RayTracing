#ifndef MATERIALH
#define MATERIALH

#include "vector.h"
#include "ray.h"
#include"hittable.h"
#include "random.h"
#include"texture.h"
#include"pdf.h"

struct ScatterRecord
{
	Ray specularRay;
	bool isSpecular;
	Vector3 attenuation;
	PDF *pdfPtr;
};

bool Refract(const Vector3& v, const Vector3& n, float ratio, Vector3& refracted) {
	Vector3 uv = UnitVector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ratio * ratio * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ratio * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else
		return false;
}

Vector3 Reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * dot(v, n) * n;
}

float Schlick(float cosine, float refIdx) {
	float r0 = (1 - refIdx) / (1 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Material
{
public:
	virtual bool Scatter(const Ray& in, const HitRecord& record,ScatterRecord& srec)const { return false; };
	virtual float ScatteringPDF(const Ray& rIn, const HitRecord& rec, const Ray& scattered) const 
	{
		return 0;
	}
	virtual Vector3 Emitted(const Ray&rIn,const HitRecord&rec, float u, float v, const Vector3& p)const 
	{
		return Vector3(0, 0, 0);
	}

};

class Lambertian :public Material
{
public:
	Lambertian(Texture* b) :albedo(b) {}
	bool Scatter(const Ray& in, const HitRecord& record, ScatterRecord& srec)const
	{
		srec.isSpecular = false;
		srec.attenuation = albedo->Value(record.u, record.v, record.p);
		srec.pdfPtr = new CosinePDF(record.normal);
		return true;
	}
	float ScatteringPDF(const Ray& rIn, const HitRecord& rec, const Ray& scattered)const 
	{
		float cosine = dot(rec.normal, UnitVector(scattered.Direction()));
		if (cosine < 0)
			return 0;
		return cosine / M_PI;
	}
	Texture * albedo;
};

class Metal :public Material
{
public:
	Metal(const Vector3& a,float f) :albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool Scatter(const Ray& in, const HitRecord& record, ScatterRecord& srec) const
	{
		Vector3 reflect = Reflect(UnitVector(in.Direction()), record.normal);
		srec.specularRay = Ray(record.p, reflect + fuzz * RandomInUnitSphere());
		srec.attenuation = albedo;
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		return true;
	}
	Vector3 albedo;
	float fuzz;
};

class DiffuseLight :public Material
{
public:
	DiffuseLight(Texture* a) :emit(a) {}
	virtual Vector3 Emitted(const Ray& rIn, const HitRecord& rec, float u, float v, const Vector3& p)const
	{
		if (dot(rec.normal, rIn.Direction()) < 0.0)
			return emit->Value(u, v, p);
		else
			return Vector3(0, 0, 0);
	}
	Texture* emit;
};

class Dielectric : public Material {
public:
	Dielectric(float ri) : refIdx(ri) {}
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const {
		srec.isSpecular = true;
		srec.pdfPtr = 0;
		srec.attenuation = Vector3(1.0, 1.0, 1.0);
		
		Vector3 outwardNormal;
		Vector3 reflected = Reflect(r_in.Direction(), rec.normal);
		float ratio;
		Vector3 refracted;
		float reflectProb;
		float cosine;
		if (dot(r_in.Direction(), rec.normal) > 0) {
			outwardNormal = -rec.normal;
			ratio = refIdx;
			cosine = /*refIdx **/ dot(r_in.Direction(), rec.normal)
				/ r_in.Direction().Length();
		}
		else {
			outwardNormal = rec.normal;
			ratio = 1.0 / refIdx;
			cosine = -dot(r_in.Direction(), rec.normal)
				/ r_in.Direction().Length();
		}
		if (Refract(r_in.Direction(), outwardNormal, ratio, refracted)) {
			reflectProb = Schlick(cosine, refIdx);
		}
		else {
			reflectProb = 1.0;
		}

		if (RandomDouble() < reflectProb) {
			srec.specularRay = Ray(rec.p, reflected);
		}
		else {
			srec.specularRay = Ray(rec.p, refracted);
		}
		return true;
	}
	float refIdx;
};
//
//class Isotropic :public Material 
//{
//public:
//	Isotropic(Texture* a) :albedo(a) {}
//	virtual bool Scatter(const Ray& rIn, const HitRecord& rec, Vector3& attenuation, Ray& scatter, float& pdf)const
//	{
//		scatter = Ray(rec.p,RandomFlect());
//		attenuation = albedo->Value(rec.u, rec.v, rec.p);
//		return true;
//	}
//	Texture* albedo;
//};
#endif