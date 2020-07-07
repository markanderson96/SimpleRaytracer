#ifndef SURFACEH
#define SURFACEH

#include "ray.h"

struct hit_record {
	double t;
	vec3 p;
	vec3 normal;
};

class surface
{
	public:
		virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif