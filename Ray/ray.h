#pragma once
#ifndef RAY_H
#define RAY_H
#include "vec3.h"
//光线类
//光线由两部分组成：射线原点与射线方向，可以用一个参数t表示线上的某个点。
//有函数origin()、direction()、at(double)分别返回光线的原点、方向、线上点t的vec3

class ray {
public:
	//光线原点
	point3 orig;
	//光线方向向量
	vec3 dir;
	//为了生成运动模糊，我们设光线在快门张开的时间内随机发射
	double tm;
public:
	//空光线，根据vec3的初始化规则，dir与orig均被置为{0,0,0}
	ray();
	//正常初始化
	ray(point3& orig, const vec3& direction, double time = 0.0) :orig(orig), dir(direction), tm(time) {}
	ray(point3&& orig, const vec3& direction, double time = 0.0) :orig(orig), dir(direction), tm(time) {}

	//基础功能函数
	point3 origin() const { return orig; }
	vec3 direction() const { return dir; }
	double time() const { return tm; }
	point3 at(double t){ return orig + t * dir; }
};

#endif