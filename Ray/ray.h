#pragma once
#ifndef RAY_H
#define RAY_H
#include "vec3.h"
//������
//��������������ɣ�����ԭ�������߷��򣬿�����һ������t��ʾ���ϵ�ĳ���㡣
//�к���origin()��direction()��at(double)�ֱ𷵻ع��ߵ�ԭ�㡢�������ϵ�t��vec3

class ray {
public:
	//����ԭ��
	point3 orig;
	//���߷�������
	vec3 dir;
	//Ϊ�������˶�ģ��������������ڿ����ſ���ʱ�����������
	double tm;
public:
	//�չ��ߣ�����vec3�ĳ�ʼ������dir��orig������Ϊ{0,0,0}
	ray();
	//������ʼ��
	ray(point3& orig, const vec3& direction, double time = 0.0) :orig(orig), dir(direction), tm(time) {}
	ray(point3&& orig, const vec3& direction, double time = 0.0) :orig(orig), dir(direction), tm(time) {}

	//�������ܺ���
	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }
	point3 at(double t){ return orig + t * dir; }
};

#endif