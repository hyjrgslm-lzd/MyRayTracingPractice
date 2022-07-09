#pragma once
#ifndef  HITTABLE_LIST_H
#define HTTABLE_LIST_H
#include "hittable.h"
#include <memory>
#include "aabb.h"
#include <vector>
//�������ڴ洢���߿��ཻ�������
//��objects�У��洢�ż̳���hittable��ĸ������塣ͨ���������е�hit���������ǿ���һ���Լ������еĹ��߽��
using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
public:
	std::vector<shared_ptr<hittable>> objects;

public:
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }
	void add(shared_ptr<hittable> object) { objects.emplace_back(object); }
	void clear() { objects.clear(); }

	virtual bool hit(ray& r, double t_min, double t_max, hit_record& rec) override;
	virtual bool bounding_box(double time0, double time1, aabb& output_box) override;
};

//�����е�object��������������У���ô�ڶ�Ӧ��object�д洢�����ཻ��Ϣ
//������rec���������ཻ��������t��С��������ཻ��Ϣ
bool hittable_list::hit(ray& r, double t_min, double t_max, hit_record& rec) {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (auto& object : objects) {
		if (object->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) {
	aabb temp_box;
	bool first_box = true;

	for (auto& object : objects) {
		if (!object->bounding_box(time0, time1, temp_box)) return false;
		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}
#endif // ! HITTABLE_LIST_H
