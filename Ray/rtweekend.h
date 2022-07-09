#pragma once
#ifndef RTWEEKEND_H
#define RTWEEKEND_H
//用于存储常用常量和实用函数的实现
#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;
const double double_miss = 0.0001;

inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }

inline double random_double(double min = 0.0, double max = 1.0) {
	static std::uniform_real_distribution<double> distribution(min, max);
	static std::mt19937 generator;
	return distribution(generator);
}

//限制x的值在min，max之间
inline double clamp(double x, double min, double max) {
	return x < min ? min : (x > max ? max : x);
}

#include "ray.h"
#include "vec3.h"
#endif