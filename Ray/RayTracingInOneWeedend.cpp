#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <thread>
#include <mutex>
#include <iostream>

color ray_color(ray& r, hittable& world, int depth) {
	hit_record rec;
	if (depth <= 0) 
		return color(0, 0, 0);

	//这里，随机的漫反射方向我们有多种方式进行采样。
	//random_in_unit_sphere()与random_unit_vector()是两种不同的随机采样方法，后者更倾向于靠近法线，但更加均匀
	//random_in_hemisphere(normal)函数是对所有角度均匀取样的一种采样方法
	if (world.hit(r, double_miss, infinity, rec)) {
		ray scattered = ray(vec3(0, 0, 0), vec3(0, 0, 0));
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}
	vec3 unit_direction = r.direction().normalized();
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

inline void UpdateProgress(float progress)
{
	int barWidth = 100;

	std::cerr << "[";
	int pos = barWidth * progress;
	for (int i = 0; i < barWidth; ++i) {
		if (i < pos) std::cerr << "=";
		else if (i == pos) std::cerr << ">";
		else std::cerr << " ";
	}
	std::cerr << "] " << progress * 100.0 << " %\r";
	std::cerr.flush();
};

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

int main() {
	//渲染设置
	bool multithread = true;

	//图像设置
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1920;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 500;
	const int max_depth = 50;

	//场景设置
	hittable_list world = random_scene();

	//相机设置
	point3 lookfrom(13, 2, 3);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	//相机设置
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0, 0, 0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);

	auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	if (!multithread) {
		for (int j = image_height - 1; j >= 0; --j) {
			UpdateProgress(((double)image_height - j) / image_height);
			for (int i = 0; i < image_width; ++i) {
				color pixel_color(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (i + random_double()) / (image_width - 1);
					auto v = (j + random_double()) / (image_height - 1);
					ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
				write_color(std::cout, pixel_color, samples_per_pixel);
			}
		}
	}

	else {
		int process = 0;
		color** colorMap = new color*[image_width];
		for (int i = 0; i < image_width; ++i) colorMap[i] = new color[image_height];
		std::mutex mutex_process, mutex_update;
		auto castRay_thread = [&](int start_width, int end_width, int start_height, int end_height) {
			camera cam1(cam);
			for (int i = start_width; i < end_width; ++i) {
				for (int j = start_height; j < end_height; ++j) {
					color pixel_color(0, 0, 0);
					for (int s = 0; s < samples_per_pixel; ++s) {
						auto u = (i + random_double()) / (image_width - 1);
						auto v = (j + random_double()) / (image_height - 1);
						ray r = cam1.get_ray(u, v);
						pixel_color += ray_color(r, world, max_depth);
					}
					colorMap[i][j] = pixel_color;
					std::lock_guard<std::mutex> g1(mutex_process);
					process++;
				}
				std::lock_guard<std::mutex> g2(mutex_update);
				UpdateProgress(1.0 * process / image_width / image_height);
			}
		};

		int thread_id = 0;
		constexpr int bx = 8;
		constexpr int by = 2;

		std::thread th[bx * by];
		int stride_x = (image_width + 1) / bx;
		int stride_y = (image_height + 1) / by;

		for (int i = 0; i < image_height; i += stride_y) {
			for (int j = 0; j < image_width; j += stride_x) {
				th[thread_id] = std::thread(castRay_thread, j, std::min(image_width, j + stride_x), i, std::min(image_height, i + stride_y));
				++thread_id;
			}
		}
		for (int i = 0; i < bx * by; ++i) th[i].join();

		for (int j = image_height - 1; j >= 0; --j) {
			for (int i = 0; i < image_width; ++i) {
				write_color(std::cout, colorMap[i][j], samples_per_pixel);
			}
		}
	}

	UpdateProgress(1);
}