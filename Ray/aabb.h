#pragma once
#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"
//是AABB轴对齐包围盒的定义
class aabb {
public:
	point3 minimum;
	point3 maximum;

public:
	aabb() {}
	aabb(point3 a, point3 b) :minimum(a), maximum(b) {}

	point3 min() const { return minimum; }
	point3 max() const { return maximum; }

	//简述这里AABB包围盒计算是否光线经过其中的方式
	//我们对x、y、z三个轴，分别计算光线经过对应这根轴两个面的时间tmin和tmax
	//三个tmin取最大，tmax取最小，如果最后的tmin小于tmax，就说明光线经过了AABB包围盒
	//如果光线和某个面平行，那么这个除法结果会是正负无穷。如果光线经过包围盒，那么一定是一正一负
	//如果光线在某个面上，那么这里的结果是NaN，这种情况我们单独处理
	//利用cmath库的fmin和fmax函数处理上述情况
	//下面这种是基本实现
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

	//采用Axis-aligned提供的优化的表达方式
	bool hit(ray& r, double t_min, double t_max) {
		for (int a = 0; a < 3; ++a) {
			auto invD = 1.f / r.direction()[a];
			auto t0 = (min()[a] - r.origin()[a]) * invD;
			auto t1 = (max()[a] - r.origin()[a]) * invD;

			//如果光线在这个方向是向负半轴传播的，那么上面的两个时间要交换顺序
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
