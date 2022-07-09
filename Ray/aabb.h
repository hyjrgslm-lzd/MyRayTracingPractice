#pragma once
#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"
//��AABB������Χ�еĶ���
class aabb {
public:
	point3 minimum;
	point3 maximum;

public:
	aabb() {}
	aabb(point3 a, point3 b) :minimum(a), maximum(b) {}

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	//��������AABB��Χ�м����Ƿ���߾������еķ�ʽ
	//���Ƕ�x��y��z�����ᣬ�ֱ������߾�����Ӧ������������ʱ��tmin��tmax
	//����tminȡ���tmaxȡ��С���������tminС��tmax����˵�����߾�����AABB��Χ��
	//������ߺ�ĳ����ƽ�У���ô���������������������������߾�����Χ�У���ôһ����һ��һ��
	//���������ĳ�����ϣ���ô����Ľ����NaN������������ǵ�������
	//����cmath���fmin��fmax���������������
	//���������ǻ���ʵ��
	/*bool hit(ray& r, double t_min, double t_max) const {
		for (int a = 0; a < 3; ++a) {
			auto t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a], 
									(maximum[a] - r.origin()[a]) / r.direction()[a]);
			auto t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a],
									(maximum[a] - r.origin()[a]) / r.direction()[a]);

			t_min = fmax(t_min, t0);
			t_max = fmin(t_max, t1);

			if (t_max <= t_min) return false;
		}
		return true;
	}*/

	//����Axis-aligned�ṩ���Ż��ı�﷽ʽ
	bool hit(ray& r, double t_min, double t_max) {
		for (int a = 0; a < 3; ++a) {
			auto invD = 1.f / r.direction()[a];
			auto t0 = (min()[a] - r.origin()[a]) * invD;
			auto t1 = (max()[a] - r.origin()[a]) * invD;

			//�������������������򸺰��ᴫ���ģ���ô���������ʱ��Ҫ����˳��
			if (invD < 0.0f)
				std::swap(t0, t1);

			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;

			if (t_max <= t_min)
				return false;
		}
		return true;
	}
};


aabb surrounding_box(aabb a, aabb b) {
	point3 minimum = vec3(fmin(a.min()[0], b.min()[0]), fmin(a.min()[1], b.min()[1]), fmin(a.min()[2], b.min()[2]));
	point3 maximum = vec3(fmin(a.max()[0], b.max()[0]), fmin(a.max()[1], b.max()[1]), fmin(a.max()[2], b.max()[2]));
	return aabb(minimum, maximum);
}
#endif // !AABB_H
