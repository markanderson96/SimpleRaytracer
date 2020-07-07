#ifndef CAMERAH
#define CAMERAH

#include "vec3.h"
#include "ray.h"

class camera
{
	public:
		camera() {
			// define camera
			const auto aspect_ratio = 16.0 / 9.0;
			auto viewport_height = 2.0;
			auto viewport_width = viewport_height * aspect_ratio;
			auto focal_length = 1.0;

			origin = vec3(0, 0, 0);
			horizontal = vec3(viewport_width, 0, 0);
			vertical = vec3(0, viewport_height, 0);
			lower_left = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
		}

		ray get_ray(double u, double v) const {
			return ray(origin, lower_left + u * horizontal + v * vertical - origin);
		}

	private:
		vec3 origin;
		vec3 lower_left;
		vec3 horizontal;
		vec3 vertical;
};

#endif