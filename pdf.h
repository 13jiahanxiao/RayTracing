#ifndef PDFH
#define PDFH

#include"vector.h"
#include"random.h"
class PDF 
{
public :
	virtual float Value(const Vector3& direction)const = 0;
	virtual Vector3 Generate()const = 0;
	virtual ~PDF() {}
};

class CosinePDF :public PDF 
{
public:
	CosinePDF(const Vector3& w) { uvw.BuildFromW(w); }
	virtual float Value(const Vector3& direction)const 
	{
		float cosine =dot(UnitVector(direction),uvw.W());
		if (cosine > 0)
			return cosine/M_PI;
		else
			return 0;
	}
	virtual Vector3 Generate() const 
	{
		return uvw.Local(RandomCosineDirection());
	}
	ONB uvw;
};

class HittablePDF :public PDF 
{
public:
	HittablePDF(Hittable* p, const Vector3& origin) :ptr(p), o(origin) {}
	virtual float Value(const Vector3& direction)const 
	{
		return ptr->PDFValue(o, direction);
	}
	virtual Vector3 Generate() const 
	{
		return ptr->Random(o);
	}
	Vector3 o;
	Hittable* ptr;
};

class MixturePDF : public PDF {
public:
	MixturePDF(PDF *p0, PDF *p1) { p[0] = p0; p[1] = p1; }
	virtual float Value(const Vector3& direction) const {
		return 0.5 * p[0]->Value(direction) + 0.5 * p[1]->Value(direction);
	}
	virtual Vector3 Generate() const {
		if (RandomDouble() < 0.5)
			return p[0]->Generate();
		else
			return p[1]->Generate();
	}
	PDF* p[2];
};


#endif
