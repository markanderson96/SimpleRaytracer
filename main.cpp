#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <fstream>
#include <vector>

#include "TinyPngOut.hpp"
#include "vec3.h"
#include "ray.h"

#define WIDTH 512
#define HEIGHT 256

using std::uint8_t;
using std::uint16_t;
using std::size_t;

bool hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;

	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	
	float discriminant = b * b - 4 * a * c;
	return (discriminant > 0);
}

vec3 colour(const ray& r) {
	if (hit_sphere(vec3(0.0, 0.0, -1.0), 0.5, r))
		return vec3(1.0, 0.0, 0.0);
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 0.8);
}

int main() {
	try {
		std::ofstream out("testpng.png", std::ios::binary);
		TinyPngOut pngout(static_cast<uint32_t>(WIDTH), static_cast<uint32_t>(HEIGHT), out);
		std::vector<uint8_t> line(static_cast<size_t>(WIDTH) * 3);

		vec3 lower_left(-2.0, -1.0, -1.0);
		vec3 horizontal(4.0, 0.0, 0.0);
		vec3 vertical(0.0, 2.0, 0.0);
		vec3 origin(0.0, 0.0, 0.0);


		for (int j = 0; j < HEIGHT; j++) {
			for (int i = 0; i < WIDTH; i++) {
				float u = float(i) / float(WIDTH);
				float v = float(j) / float(HEIGHT);

				ray r(origin, lower_left + u * horizontal + v * vertical);

				vec3 col = colour(r);

				int ir = int(col.r() * 255.99);
				int ig = int(col.g() * 255.99);
				int ib = int(col.b() * 255.99);

				line[i * 3 + 0] = ir;
				line[i * 3 + 1] = ig;
				line[i * 3 + 2] = ib;
			}
			pngout.write(line.data(), static_cast<size_t>(WIDTH));
		}
		return EXIT_SUCCESS;
	}
	catch (const char *msg) {
		std::cerr << msg << std::endl;
		return EXIT_FAILURE;
	}
}