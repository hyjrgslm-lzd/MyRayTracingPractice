#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H
//hittable.h��һ�������࣬����һ�����ܹ������еĺ�����������е�󷵻���Ϣ�Ľṹ��
//���Ǻ��洴������Ļ��������������������ײʱ��ͨ������ĺ���ʵ��

//Ϊ����Ӷ��ֲ��ʣ��������ӿ�������material���Լ�һ��ָ������ָ�� 

#include "ray.h"
#include "rtweekend.h"
#include "aabb.h"

class material;

//����������к󷵻صĽṹ��
//������ֻ�ṩһ�ֽ�һϵ�в�����䵽�ṹ�еķ�����
struct hit_record {
	//�������еĵ�
	point3 p;
	//��������ķ���
	vec3 normal;
	//ָ����ϵ�ָ��
	shared_ptr<material> mat_ptr;
	//��������ʱ��t
	double t;
	//���������Ƿ����յ��������ı��棨�����ߴ��������յ�������棩
	bool front_face;
	//���ݹ��ߺͷ��ߵ�˵�������ȷ�����ߴ�����ָ��������ķ��򣬲�������Ϊnormal
	//ʵ���ϣ��÷���ʼ��ָ����߷���ͷ���ʼ��ָ��������࣬��Ӧ����ɫʱ���Ǽ��ν׶��ж�����ķ���
	//��Ϊ����ʼ��ָ������������������⣬�����Ͻ϶࣬�����������ʵ��
	void set_face_normal(const ray r, const vec3 outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

//������������࣬���а���һ����������Ƿ����в�����hit_record�ṹ��ĺ���
//�ھ������ʱ��������ʵ�ֶ�Ӧ��hit������������ﶨ��Ϊ���麯��
class hittable {
public:
	virtual bool hit(ray& r, double t_min, double t_max, hit_record& rec)=0;

	//�ڶ���AABB��Χ�к�����Ҫ���һ�����������㵱ǰ������Ŀ��İ�Χ��
	//�ڴ˻����ϣ����Ǵ���BVH�ṹ��������л���
	//����ʵ���ϵ��������ᱻ���䵽Ҷ�ӽڵ���
	//��������һ��bool����Ϊ��һЩ���壬��������ƽ�棬����Ҫ��Χ��
	//�����˶����壬Ҫ��һ���ܹ���Χס�������˶��ռ�İ�Χ��
	virtual bool bounding_box(double time0, double time1, aabb& output_box) = 0;
};
#endif