#pragma once
#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"
#include "hittable.h"
#include "aabb.h"

//这是为了实现运动模糊而实现的，位置会随时间变化的球体类

class moving_sphere : public hittable {
public:
	point3 center0, center1;
	double time0, time1;
	double radius;
	shared_ptr<material> mat_ptr;

public:
	moving_sphere() {}
	moving_sphere(
		point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m) 
		: center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m) 
	{}

	virtual bool hit(ray& r, double t_min, double t_max, hit_record& rec) override;
	virtual bool bounding_box(double _time0, double _time1, aabb& output_box) override;

	point3 center(double time); 
};

point3 moving_sphere::center(double time) {
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}


bool moving_sphere::hit(ray& r, double t_min, double t_max, hit_record& rec) {
	auto a = dot(r.direction(), r.direction());
	auto half_b = dot(r.origin() - center(r.time()), r.direction());
	auto c = dot(r.origin() - center(r.time()), r.origin() - center(r.time())) - radius * radius;

	auto delta = pow(half_b, 2) - a * c;
	if (delta < 0) return false;

	else {
		auto sqrted = sqrt(delta);
		//我们希望找到光线第一次命中的点，也就是t相对较小的点。因为sqrted一定是非负值，所以我们
		//先求-b-sqrted，如果非法再测另一个点
		auto root = (-half_b - sqrted) / a;
		if (root < t_min || root > t_max) {
			root = (-half_b + sqrted) / a;
			if (root < t_min || root > t_max) return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.normal = (rec.p - center(r.time())).normalized();
		//添加判定表面方向的代码，让rec的normal始终指向物体外侧
		vec3 outward_normal = (rec.p - center(r.time())) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = mat_ptr;

		return true;
	}
}

bool moving_sphere::bounding_box(double _time0, double _time1, aabb& output_box) {
	aabb box0(center(_time0) - vec3(radius, radius, radius), center(_time0) + vec3(radius, radius, radius));
	aabb box1(center(_time1) - vec3(radius, radius, radius), center(_time1) + vec3(radius, radius, radius));
	output_box = surrounding_box(box0, box1);
	return true;
}
#endif // !MOVING_SPHERE_H
