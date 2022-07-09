#pragma once
#ifndef SPHERE_H
#define SPHERE_H
//���࣬�ǻ���hittable����չ����һ����������
#include "hittable.h"
#include "vec3.h"

class sphere :public hittable {
public:
	//�������ĺͰ뾶
	point3 center;
	double radius = 0;
	//����Ĳ��ʵ�ָ�롣
	shared_ptr<material> mat_ptr;
public:
	//���幹�캯������������hit��������д
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
		//����ϣ���ҵ����ߵ�һ�����еĵ㣬Ҳ����t��Խ�С�ĵ㡣��Ϊsqrtedһ���ǷǸ�ֵ����������
		//����-b-sqrted������Ƿ��ٲ���һ����
		auto root = (-half_b - sqrted) / a;
		if (root < t_min || root > t_max) {
			root = (-half_b + sqrted) / a;
			if (root < t_min || root > t_max) return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.normal = (rec.p - center).normalized();
		//����ж����淽��Ĵ��룬��rec��normalʼ��ָ���������
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
