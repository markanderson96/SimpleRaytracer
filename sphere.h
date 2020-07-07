#ifndef SPHEREH
#define SPHEREH

#include <iostream>
#include "surface.h"
#include "vec3.h"

class sphere : public surface {
	public:
		sphere() {}
		sphere(vec3 cen, double r) : center(cen), radius(r) {};
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

	public:
		vec3 center;
		double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;

	auto a = r.direction().length_squared();
	auto b = dot(oc, r.direction());
	auto c = oc.length_squared() - radius * radius;

	auto discriminant = (b * b) - (a * c);

	if (discriminant > 0) {
		auto temp = (-b - sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_t(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_t(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}

#endif

