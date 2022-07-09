#pragma once
#ifndef SPHERE_H
#define SPHERE_H
//球类，是基于hittable类拓展出的一个几何体类
#include "hittable.h"
#include "vec3.h"

class sphere :public hittable {
public:
	//定义球心和半径
	point3 center;
	double radius = 0;
	//球体的材质的指针。
	shared_ptr<material> mat_ptr;
public:
	//定义构造函数，并声明对hit函数的重写
	sphere() {}
	sphere(point3 cent, double r, shared_ptr<material> m) :center(cent), radius(r), mat_ptr(m) {}
	
	virtual bool hit(ray& r, double t_min, double t_max, hit_record& rec) override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) override;
};

bool sphere::hit(ray& r, double t_min, double t_max, hit_record& rec) {
	auto a = dot(r.direction(), r.direction());
	auto half_b = dot(r.origin() - center, r.direction());
	auto c = dot(r.origin() - center, r.origin() - center) - radius * radius;

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
		rec.normal = (rec.p - center).normalized();
		//添加判定表面方向的代码，让rec的normal始终指向物体外侧
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat_ptr = mat_ptr;

		return true;
	}
}

bool sphere::bounding_box(double time0, double time1, aabb& output_box) {
	output_box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}


#endif
