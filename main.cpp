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
#include "surface.h"
#include "surfacelist.h"
#include "sphere.h"
#include "camera.h"
#include "utility.h"

using std::uint8_t;
using std::uint16_t;
using std::size_t;

// return ray colour
vec3 ray_colour(const ray& r, const surface& world) {
	hit_record rec;
	if (world.hit(r, 0, 1E32, rec)) {
		return 0.5 * (rec.normal + vec3(1, 1, 1));
	}
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	// define camera
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_height = 240;
	const int image_width = static_cast<int>(image_height * aspect_ratio);

	// samples
	const int samples_per_pixel = 100;

	// create a surface list
	surface_list world;
	world.add(make_shared<sphere>(vec3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(vec3(-1.0, 0, -1.5), 0.5));
	world.add(make_shared<sphere>(vec3(1.0, 0, -1.5), 0.5));
	world.add(make_shared<sphere>(vec3(0, -100.5, -1), 100));

	camera cam;

	//try to write out to PNG
	try {
		// setup PNG
		std::ofstream out("testpng.png", std::ios::binary);
		TinyPngOut pngout(static_cast<uint32_t>(image_width), static_cast<uint32_t>(image_height), out);
		std::vector<uint8_t> line(static_cast<size_t>(image_width) * 3);

		// cycle through pixels
		for (int j = image_height - 1; j >= 0; j--) { // y-direction
			for (int i = 0; i < image_width; i++) { // x-direction
				vec3 col(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; s++) {
					auto u = double(i + random_double()) / (image_width - 1);
					auto v = double(j + random_double()) / (image_height - 1);
					ray r = cam.get_ray(u, v);
					col += ray_colour(r, world);
				}

				auto scale = 1.0 / samples_per_pixel;
				col *= scale;

				// conversion to 8-bit int
				line[i * 3 + 0] = static_cast<int>(256 * clamp(col.r(), 0.0, 0.999));
				line[i * 3 + 1] = static_cast<int>(256 * clamp(col.g(), 0.0, 0.999));
				line[i * 3 + 2] = static_cast<int>(256 * clamp(col.b(), 0.0, 0.999));
			}
			pngout.write(line.data(), static_cast<size_t>(image_width));
		}
		return EXIT_SUCCESS;
	}
	// png exception
	catch (const char *msg) {
		std::cerr << msg << std::endl;
		return EXIT_FAILURE;
	}
}